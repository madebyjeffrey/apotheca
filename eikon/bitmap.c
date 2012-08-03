
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define PNG_DEBUG 4
#include "png.h"

#include "eikon.h"

void *(memcpy)(void *restrict b, const void *restrict a, size_t n)
{
    char *s1 = b;
    const char *s2 = a;
    for(; 0<n; --n)
        *s1++ = *s2++;
    return b;
}

// # of pixels
static size_t bmp_nelements(bmp *b)
{
    return bmp_cx(b) * bmp_cy(b);
}

static size_t bmp_nbytes(bmp *b)
{
    return bmp_nelements(b) * sizeof(pixel_t);
}

unsigned bmp_cx(bmp *b)
{
    if (b) return b->width;
    return 0;
}

unsigned bmp_cy(bmp *b)
{
    if (b) return b->height;
    return 0;
}

// bmp_init: create new bitmap sized width, height
// output:   allocated bitmap, uninitialized
bmp *bmp_init(unsigned width, unsigned height)
{
    // check for reasonable size image:
    // somewhat arbitrary - 1 billion pixels
    if (width * height > 1000000000) return NULL;
    
   bmp *b = malloc(sizeof(struct tagBitmap));

   if (!b) return NULL;

   b->width = width;
   b->height = height;
   
//   printf("width: %ld   height: %ld    allocated memory: %ld\n",
//             width, height, width * height * 4);

   b->data = malloc(bmp_nbytes(b));

   if (!b->data)
   {
      free(b);
      return NULL;
   }

//   memset(b->data, 0, width * height * sizeof(pixel_t));

   return b;
}

void   bmp_free(bmp *b)
{
    if (b)
    {
        if (b->data)
        {
            free(b->data);
            b->data = NULL;
        }
        
        free(b);
    }
}

// bmp_dup: duplicate b
// output:  copy of b
bmp *  bmp_dup(bmp * b)
{
    if (!b) return NULL;
    
    bmp *b2 = bmp_init(bmp_cx(b), bmp_cy(b));
    
    if (!b2) return NULL;
    
    memcpy(b2->data, b->data, bmp_nbytes(b));
    
    return b2;
}

bmp *  bmp_fill(bmp *b, pixel_t colour)
{
    if (!b) return NULL;
    
    bmp *b2 = bmp_init(bmp_cx(b), bmp_cy(b));
    
    if (!b2) return NULL;
    
    for (size_t i = 0; i < bmp_nelements(b); ++i)
    {
        b2->data[i] = colour;
    }
        
    return b2;
}

bool bmp_put(bmp *dst, unsigned x, unsigned y, pixel_t colour)
{
    if (!dst) return false;
    
    if (x < bmp_cx(dst) && y < bmp_cy(dst))
    {
        dst->data[y * dst->width + x] = colour;
        return true;
    }
    
    return false;
}

pixel_t bmp_get(bmp *src, size_t x, size_t y)
{
    if (!src) return 0;
    
    if (x < src->width && y < src->height)
    {
        return src->data[y * src->width + x];
    }
    
    return false;
}

// bmp_cpy: copy b2 into b1 @ location x, y
// output:  copy of b1 modified
bmp *  bmp_cpy(bmp * restrict b1, bmp * restrict b2, unsigned x, unsigned y)
{
    if (!(b1 && b2)) return NULL;
    
    return NULL;   
}


