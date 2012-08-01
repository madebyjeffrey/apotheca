
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define PNG_DEBUG 4
#include "png.h"

#include "bitmap.h"

Bitmap createBitmap(size_t width, size_t height)
{
    // check for reasonable size image:
    // somewhat arbitrary - 1 billion pixels
    if (width * height > 1000000000) return NULL;
    
   Bitmap b = malloc(sizeof(struct tagBitmap));

   if (!b) return NULL;

   b->width = width;
   b->height = height;
   
//   printf("width: %ld   height: %ld    allocated memory: %ld\n",
//             width, height, width * height * 4);

   b->data = malloc(width * height * sizeof(pixel_t));

   if (!b->data)
   {
      free(b);
      return NULL;
   }

   memset(b->data, 0, width * height * sizeof(pixel_t));

   return b;
}

void deleteBitmap(Bitmap b)
{
    if (b)
    {
        if (b->data)
        {
            free(b->data);
        }
        
        free(b);
    }
}

bool enlargeBitmap(Bitmap b, size_t w2, size_t h2, pixel_t colour)
{
    if (!b) return false;
    
    if (w2 < b->width) 
        return false;
    if (h2 < b->height)
        return false;
        
    // we are now resizing on at least one dimension
    
    pixel_t *newdata;
    
    newdata = malloc(w2 * h2 * sizeof(pixel_t));
    
    if (!newdata)
        return false;
        
    // copy from 0 to b->height from old to new
    for (size_t i = 0; i < b->height; ++i)
    {
        memcpy(newdata + (i * w2), 
               b->data + (i * b->width), 
               b->width * 4);
        if (w2 > b->width)
        {
            for (size_t j = b->width; j < w2; ++j)
            {
                newdata[i * w2 + j] = colour;
            }
        } 
    }
    
    // fill the rest 
    for (size_t i = b->height; i < h2; ++i)
    {
        for (size_t j = 0; j < w2; ++j)
        {
            newdata[i * w2 + j] = colour;
        }
    }
    
    b->width = w2;
    b->height = h2;
    
    // replace with the new data
    free(b->data);
    b->data = newdata;
    
    return true;
}

bool fillBitmap(Bitmap b, pixel_t colour)
{
    if (!b) return false;
    
//    printf("%ld\n", sizeof(pixel_t));
    for (size_t i = 0; i < (b->width * b->height); ++i)
    {
  //      printf("index: %ld   \r", i);
        b->data[i] = colour;
    }
        
    return true;
}

bool putPixel(Bitmap dst, size_t x, size_t y, pixel_t colour)
{
    if (!dst) return false;
    
    if (x < dst->width && y < dst->height)
    {
        dst->data[y * dst->width + x] = colour;
        return true;
    }
    
    return false;
}

bool getPixel(Bitmap src, size_t x, size_t y, pixel_t *colour)
{
    if (!src || !colour) return false;
    
    if (x < src->width && y < src->height)
    {
        *colour = src->data[y * src->width + x];
        return true;
    }
    
    return false;
}

bool blitBitmap(Bitmap dst, size_t dx, size_t dy, 
                Bitmap src, size_t sx, size_t sy, size_t scx, size_t scy)
{
    if (!dst || !src) return false;
    
    // dx, dy must be within the image dst
    if (!(dx < dst->width) && !(dy < dst->height)) return false;
    
    // sx, sy must be within the image src
    if (!(sx < src->width) && !(sy < src->height)) return false;
    
    // how much of the width of src will meet the destination
    size_t sw = dst->width - dx;
    size_t sh = dst->height - dy;
    
    // maximum width
    if (sw > scx) sw = scx;
    if (sh > scy) sh = scy;
    
    // copy all the pixels from the source to the destination
    for (size_t i = 0; i < sw; ++i)
        for (size_t j = 0; j < sh; ++j)
        {
            pixel_t colour;
            
            getPixel(src, i, j, &colour);
            putPixel(dst, dx + i, dy + j, colour); 
        }
        
    return true;
}

// Bilinear Sample
bool sampleBitmapBL(Bitmap src, float x, float y, pixel_t *colour)
{
    pixel_t A, B, C, D;
    
    getPixel(src, floorl(x), floorl(y), &A);
    getPixel(src, ceill(x), floorl(y), &B);
    getPixel(src, floorl(x), ceill(y), &C);
    getPixel(src, ceill(x), ceill(y), &D);

    float dummy;
    float w = modff(x, &dummy);
    float h = modff(y, &dummy);
    float onelessh = 1 - h;
    float onelessw = 1 - w;
    float wh = w * h;
    
    float Am = onelessh * onelessw;
    float Bm = w * onelessh;
    float Cm = h * onelessw;
    float Dm = wh;
    
    unsigned char pR = Red(A) * Am
                     + Red(B) * Bm
                     + Red(C) * Cm
                     + Red(D) * Dm;

    unsigned char pG = Green(A) * Am
                     + Green(B) * Bm
                     + Green(C) * Cm
                     + Green(D) * Dm;
                     
    unsigned char pB = Blue(A) * Am
                     + Blue(B) * Bm
                     + Blue(C) * Cm
                     + Blue(D) * Dm;

    unsigned char pA = Alpha(A) * Am
                     + Alpha(B) * Bm
                     + Alpha(C) * Cm
                     + Alpha(D) * Dm;    
                     
//    *colour = A * (1 - h) * (1 - w) + B * w * (1 - h) + C * h * (1 - w) + D * w * h;
    *colour = Pixel(pR, pG, pB, pA);
    
    return true;
}

bool resizeBitmap(Bitmap src, size_t cx, size_t cy)
{
    if (!src) return false;
    
    Bitmap dst = createBitmap(cx, cy);
    
    if (!dst) return false;
    
    pixel_t colour;
    
    for (size_t i = 0; i < cx; ++i)
    {
        for (size_t j = 0; j < cy; ++j)
        {
            sampleBitmapBL(src, i * ((float)src->width / cx),
                              j * ((float)src->height / cy), 
                              &colour);
            putPixel(dst, i, j, colour);
        }
    }
    
    free(src->data);
    src->data = dst->data;
    src->width = dst->width;
    src->height = dst->height;

    return true;
}

bool saveBitmap(Bitmap src, const char *filename)
{
    if (!src) return false;
    bool code = true;
   FILE *fp = NULL;
   png_structp png_ptr = NULL;
   png_infop info_ptr = NULL;
   png_bytep row = NULL;

    // Open file for writing (binary mode)
   fp = fopen(filename, "wb");
   if (fp == NULL)
   {
      fprintf(stderr, "Could not open file %s for writing\n", filename);
      code = false;
      goto finalize;
   }
   
   // Initialize write structure
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL)
   {
      fprintf(stderr, "Could not allocate write struct\n");
      code = false;
      goto finalize;
   }
   
   // Initialize info structure
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fprintf(stderr, "Coult not allocate info struct\n");
      code = false;
      goto finalize;
   }
   
   // Setup Exception handling  (WTF!!)
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      fprintf(stderr, "Error during png create\n");
      code = false;
      goto finalize;
   }
   
   // Write header (8 bit colour depth)
   png_init_io(png_ptr, fp);
   
   png_set_IHDR(png_ptr, info_ptr, src->width, src->height,
                8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  
  // meta data that I don't care about 
/*   if (title != NULL)
   {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
   }*/
   
   png_write_info(png_ptr, info_ptr);

// Allocate memory for one row (4 bytes per pixel - RGB + A)
   const int bytesPerPixel = sizeof(pixel_t);
   row = (png_bytep) malloc(bytesPerPixel * src->width * sizeof(png_byte));
   
   for (int y = 0; y < src->height; ++y)
   {
      for (int x = 0; x < src->width; ++x)
      {
// copy RGBa source to the png
         png_byte *pngbyte = &(row[x*bytesPerPixel]);
         unsigned char *sourcebytes = (unsigned char*)src->data + bytesPerPixel*(y*src->width + x);
         pngbyte[0] = sourcebytes[0];
         pngbyte[1] = sourcebytes[1];
         pngbyte[2] = sourcebytes[2];
         pngbyte[3] = sourcebytes[3];
      }
      png_write_row(png_ptr, row);
   }
   
   png_write_end(png_ptr, NULL);
   
  finalize:
   if (fp != NULL) fclose(fp);
   if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
   if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
   if (row != NULL) free(row);
   
   return code;
}


bool loadBitmap(Bitmap *dst, const char *filename)
{
    FILE *file;
    size_t qty = 0;
    
    png_byte pngsig[8];
    bool is_png = false;
    fprintf(stderr, "Open <%s>\n", filename);
    file = fopen(filename, "rb");
    
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filename);
        return false;
        
    }

    // start check signature
    qty = fread(pngsig, 1, 8, file);
    
    if (qty != 8) 
    {
        fprintf(stderr, "Could not read 8 bytes\n");
        if (feof(file) != 0)
        {
            fprintf(stderr, "EOF\n");
        }
        fclose(file);
        return false;
    }
    

    is_png = !png_sig_cmp(pngsig, 0, 8);
    if (!is_png)
    {
        fprintf(stderr, "Signature not good\n");
        fclose(file);
        return false;
    }
    
    // end check signature
    
    
    // at this point, we know it is a png file
    // create the read structure, the nulls can be used for custom error
    // handling
    png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (!pngPtr)
    {
        fprintf(stderr, "libpng read structure could not be created.\n");
        fclose(file);
        return false;
    }
    
    // info struct created
    png_infop infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr)
    {
        fprintf(stderr, "libpng info structure could not be created.\n");
        png_destroy_read_struct(&pngPtr, 0, 0);
        return false;
    }
    
    png_infop endInfo = png_create_info_struct(pngPtr);
    if (!endInfo)
    {
        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
        return false;
    }
    
    // dirty error handling
    png_bytep *rowPtr = NULL;
    char *data = NULL;
    
    if (setjmp(png_jmpbuf(pngPtr)))
    {
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
        fprintf(stderr, "error reading info\n");
        fclose(file);
        return false;
    }
    
    // let libpng reuse our FILE*
    png_init_io(pngPtr, file);
    
    // we read 8 bytes, let it know
    png_set_sig_bytes(pngPtr, 8);
    
    png_read_info(pngPtr, infoPtr);
    
    int width = png_get_image_width(pngPtr, infoPtr);
    int height = png_get_image_height(pngPtr, infoPtr);
    png_byte colourType = png_get_color_type(pngPtr, infoPtr);
    png_byte bit_depth = png_get_bit_depth(pngPtr, infoPtr);
    png_byte channels = png_get_channels(pngPtr, infoPtr);
    
    int passes = png_set_interlace_handling(pngPtr);
    png_read_update_info(pngPtr, infoPtr);
    
    fprintf(stderr, "Size (%d, %d), colour type: %d, depth: %d, channels: %d\n",
        width, height, (int)colourType, (int)bit_depth, (int)channels);
    
    if (colourType != PNG_COLOR_TYPE_RGBA || bit_depth != 8)
    {
        fprintf(stderr, "Image is not an RGBA/32 image. No transforms supported yet.\n");
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
        return false;
    }
        
    // don't keep any unknown chunks
    png_set_keep_unknown_chunks(pngPtr, 
        PNG_HANDLE_CHUNK_NEVER,
        NULL,
        0);
    
    
//    png_read_png(pngPtr, infoPtr, PNG_TRANSFORM_IDENTITY, NULL);
    
    png_bytep *row_pointers = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; ++y)
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(pngPtr, infoPtr));

    if (setjmp(png_jmpbuf(pngPtr)))
    {
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
        fprintf(stderr, "error reading image data\n");
        fclose(file);
        return false;
    }

    png_read_image(pngPtr, row_pointers);    
    
    Bitmap b = createBitmap(width, height);
    
    if (!b)
    {
        fprintf(stderr, "could not allocate bitmap");
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
        fclose(file);

        return false;
    }

    png_byte *base = NULL;
    unsigned char c = 255;
    
    for (size_t i = 0; i < width; ++i)
        for (size_t j = 0; j < height; ++j)
        {
            base = &row_pointers[j][i*4];
            
            putPixel(b, i, j, Pixelp(base+0, base+1, base+2, &c));
            //Pixelp(base, base+1, base+2, base+3));
        }

    *dst = b;
    
    for (int y = 0; y < height; ++y)
    {
        free(row_pointers[y]);
    }
    
    free(row_pointers);
            
    png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
    fclose(file);
    return true;
}

    