
#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef uint32_t pixel_t;

struct tagBitmap
{
   size_t width;
   size_t height;

   pixel_t *data;
};

typedef struct tagBitmap *Bitmap;

Bitmap createBitmap(size_t width, size_t height);
void deleteBitmap(Bitmap b);
bool enlargeBitmap(Bitmap b, size_t w2, size_t h2, pixel_t colour);
bool fillBitmap(Bitmap b, pixel_t colour);
bool blitBitmap(Bitmap dst, size_t dx, size_t dy, 
                Bitmap src, size_t sx, size_t sy, size_t scx, size_t scy);
                
bool saveBitmap(Bitmap src, const char *filename);
bool loadBitmap(Bitmap *dst, const char *filename);

bool putPixel(Bitmap dst, size_t x, size_t y, pixel_t colour);
bool getPixel(Bitmap src, size_t x, size_t y, pixel_t *colour);

bool resizeBitmap(Bitmap src, size_t cx, size_t cy);

#define  Pixel(r, g, b, a)  (a << 24 | b << 16 | g << 8 | r)
#define  Pixelp(r, g, b, a)  (*(a) << 24 | *(b) << 16 | *(g) << 8 | *(r))


#define  Red(c) (c & 0xff)
#define  Blue(c) ((c & 0xff00) >> 8)
#define  Green(c) ((c & 0xff0000) >> 16)
#define  Alpha(c) ((c & 0xff000000) >> 24)