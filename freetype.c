#include <limits.h>
#include <stdio.h>
#include <ft2build.h>
#include <freetype/ftglyph.h>
#include <freetype/ftbitmap.h>
#include "png.h"
#include FT_FREETYPE_H
#include <math.h>
#include <float.h>

// taken from: http://www.labbookpages.co.uk/software/imgProc/libPNG.html
int writeImage(char *filename, int width, int height, unsigned char
               *buffer, char *title)
{
   int code = 0;
   FILE *fp = NULL;
   png_structp png_ptr = NULL;
   png_infop info_ptr = NULL;
   png_bytep row = NULL;

   fp = fopen(filename, "wb");
   if (fp == NULL)
   {
      fprintf(stderr, "Could not open file %s for writing\n", filename);
      code = 1;
      goto finalize;
   }
   
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL)
   {
      fprintf(stderr, "Could not allocate write struct\n");
      code = 1;
      goto finalize;
   }
   
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fprintf(stderr, "Coult not allocate info struct\n");
      code = 1;
      goto finalize;
   }
   
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      fprintf(stderr, "Error during png create\n");
      code = 1;
      goto finalize;
   }
   
   png_init_io(png_ptr, fp);
   
   png_set_IHDR(png_ptr, info_ptr, width, height,
                8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
   
   if (title != NULL)
   {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
   }
   
   png_write_info(png_ptr, info_ptr);

// Allocate memory for one row (4 bytes per pixel - RGB + A)
   const int bytesPerPixel = 4;
   row = (png_bytep) malloc(bytesPerPixel * width * sizeof(png_byte));
   
   for (int y = 0; y < height; ++y)
   {
      for (int x = 0; x < width; ++x)
      {
// copy RGBa source to the png
         png_byte *pngbyte = &(row[x*bytesPerPixel]);
         unsigned char *sourcebytes = buffer + bytesPerPixel*(y*width + x);
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

unsigned char *samplerBuffer;
int samplerPitch;
int samplerMaxX;
int samplerMaxY;


// returns either zero or non-zero
// ref to struct:
//http://www.freetype.org/freetype2/docs/reference/ft2-basic_types.html#FT_Bitmap
int sampler(int x, int y)
{
   if (x < 0 || x >= samplerMaxX || y < 0 || y >= samplerMaxY) return 0;
   unsigned char mask = 1 << (7 - (x % 8));
// big or little endian???
   return samplerBuffer[(y*samplerPitch) + x/8] & mask;
}

int main()
{
   FT_Library library;
   int error;
   
   error = FT_Init_FreeType(&library);
   if (error)
   {
      fprintf(stderr, "Cannot init FreeType Library");
   }
   
   FT_Face face;
   
   error = FT_New_Face(
      library,
      "CPMono_v07_Light.otf",
      0,
      &face);
   
   if (error == FT_Err_Unknown_File_Format)
   {
      fprintf(stderr, "Unsupported or invalid font file");
   }
   else if (error)
   {
      fprintf(stderr, "Cannot open or read file, or format is invalid");
   }
   
   error = FT_Set_Pixel_Sizes(face, 0, 2048);
   
   if (error)
   {
      fprintf(stderr, "Cannot set font size");
   }
   
   for (char i = 'a'; i < 'z'; ++i)
   {
      int glyph_index = FT_Get_Char_Index(face, i);
      error = FT_Load_Glyph(face, glyph_index, 0);
      if (error)
      {
         fprintf(stderr, "No such Glyph");
      }
      error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
      if (error)
      {
         fprintf(stderr, "Cannot render glpyh");
      }
      
      FT_Bitmap bitmap = face->glyph->bitmap;
      samplerBuffer = bitmap.buffer; // global
      samplerPitch = bitmap.pitch; // global
      samplerMaxX = bitmap.width; // global
      samplerMaxY = bitmap.rows; // global

/*
      unsigned long *pixmap = malloc(bitmap.width * bitmap.rows);

      if (!pixmap)
      {
         fprintf(stderr, "memory error on pixmap\n");
         exit(1);
      }
*/
      
//      FT_Bitmap pixmap;

//      FT_Bitmap_Convert(library, &bitmap, &pixmap, 1);
      
      
      char *image = malloc(35);
      strcpy(image, "fontglyph");
      char name[] = { i, '\0' };
      strcat(image, name);
      strcat(image, ".png");

      writeImage(image, samplerMaxX, samplerMaxY, samplerBuffer, NULL);

      free(image); 
   }
}
