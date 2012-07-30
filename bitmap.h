
#pragma once

#include <stdlib.h>

struct tagBitmap
{
   size_t width;
   size_t height;

   unsigned long *data;
};

typedef struct tagBitmap *Bitmap;

Bitmap createBitmap(size_t width, size_t height);
void deleteBitmap(Bitmap b);
bool resizeBitmap(Bitmap b, size_t w2, size_t h2, unsigned long colour);
bool fillBitmap(Bitmap b, unsigned long colour);
bool blitBitmap(Bitmap dst, size_t dx, size_t dy, 
                Bitmap src, size_t sx, size_t sy, size_t scx, size_t scy);
                
bool saveBitmap(Bitmap src, const char *filename);

#define  Pixel(r, g, b, a)  r << 24 | g << 16 | b << 8 | a
