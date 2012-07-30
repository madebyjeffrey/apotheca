
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

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
