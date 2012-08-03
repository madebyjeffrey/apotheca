//  Eikon Image Library
//  (C) 2012 By Jeffrey Drake
//  Licensed under the BSD License

// General principle of the library:
// All functions intended for public use
// are meant to operate in an immutable
// copying fashion.
// putPixel is a notable exception
// getPixel also is different

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

typedef struct tagBitmap bmp;

unsigned bmp_cx(bmp *b);
unsigned bmp_cy(bmp *b);

// bmp_init: create new bitmap sized width, height
// output:   allocated bitmap
bmp *bmp_init(unsigned width, unsigned height);

// bmp_free: release all b
void   bmp_free(bmp *b);

// bmp_dup: duplicate b
// output:  copy of b
bmp *  bmp_dup(bmp * b);

// bmp_fill: fill b with colour
// output:   copy of b modified
bmp *  bmp_fill(bmp *b, pixel_t colour);


// bmp_cpy: copy b2 into b1 @ location x, y
// output:  copy of b1 modified
//bmp *  bmp_cpy(bm * restrict b1, bmp * restrict b2, unsigned x, unsigned y);
//bmp *  bmp_crop(bmp * b, unsigned x, unsigned y, unsigned cx, unsigned cy);

pixel_t bmp_and(pixel_t x, pixel_t y);
pixel_t bmp_or(pixel_t x, pixel_t y);
pixel_t bmp_xor(pixel_t x, pixel_t y);
pixel_t bmp_nor(pixel_t x, pixel_t y);
pixel_t bmp_nand(pixel_t x, pixel_t y);
pixel_t bmp_xnor(pixel_t x, pixel_t y);
pixel_t bmp_first(pixel_t x, pixel_t y);
pixel_t bmp_second(pixel_t x, pixel_t y);

typedef pixel_t (*blitop)(pixel_t, pixel_t);

bmp *  bmp_blit(bmp * restrict b1, bmp * restrict b2, unsigned dx, unsigned dy,
                unsigned sx, unsigned sy, unsigned scx, unsigned scy, blitop op);

bool bmp_put(bmp *dst, unsigned x, unsigned y, pixel_t colour);
pixel_t bmp_get(bmp *src, size_t x, size_t y);

typedef pixel_t (*sampler)(bmp*, float, float);
bmp *bmp_resize(bmp *src, size_t cx, size_t cy, sampler s);
pixel_t bmp_sample_bilinear(bmp *src, float x, float y);
pixel_t bmp_sample_nearestNeighbour(bmp *src, float x, float y);


#define  Pixel(r, g, b, a)  (a << 24 | b << 16 | g << 8 | r)
#define  Pixelp(r, g, b, a)  (*(a) << 24 | *(b) << 16 | *(g) << 8 | *(r))


#define  Red(c) (c & 0xff)
#define  Blue(c) ((c & 0xff00) >> 8)
#define  Green(c) ((c & 0xff0000) >> 16)
#define  Alpha(c) ((c & 0xff000000) >> 24)

bool bmp_write(bmp *src, const char *filename);
bmp *bmp_read(const char *filename);

