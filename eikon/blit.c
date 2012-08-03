#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define PNG_DEBUG 4
#include "png.h"

#include "eikon.h"

pixel_t bmp_and(pixel_t x, pixel_t y)
{
    return x & y;
}

pixel_t bmp_or(pixel_t x, pixel_t y)
{
    return x | y;
}

pixel_t bmp_xor(pixel_t x, pixel_t y)
{
    return x ^ y;
}

pixel_t bmp_nor(pixel_t x, pixel_t y)
{
    return !(x | y);
}

pixel_t bmp_nand(pixel_t x, pixel_t y)
{
    return !(x & y);
}
pixel_t bmp_xnor(pixel_t x, pixel_t y)
{
    return !(x ^ y);
}

pixel_t bmp_first(pixel_t x, pixel_t y)
{
    return x;
}

pixel_t bmp_second(pixel_t x, pixel_t y)
{
    return y;
}

bmp *  bmp_blit(bmp * restrict b1, bmp * restrict b2, unsigned dx, unsigned dy,
                unsigned sx, unsigned sy, unsigned scx, unsigned scy, blitop op)
{
    if (op == NULL) op = bmp_and;
    
    if (!b1 || !b2) return NULL;

    // dx, dy must be within the image dst
    if (!(dx < bmp_cx(b1)) && !(dy < bmp_cy(b1))) return NULL;
    
    // sx, sy must be within the image src
    if (!(sx < bmp_cx(b2)) && !(sy < bmp_cy(b2))) return NULL;
    
    // how much of the width of src will meet the destination
    size_t sw = bmp_cx(b1) - dx;
    size_t sh = bmp_cy(b1) - dy;
    
    // maximum width
    if (sw > scx) sw = scx;
    if (sh > scy) sh = scy;
    
    // create final destination
    bmp *dst = bmp_dup(b1);
    
    if (!dst) return NULL;

    pixel_t b1_colour;    
    pixel_t b2_colour;

    // copy all the pixels from the source to the destination
    for (size_t i = 0; i < sw; ++i)
        for (size_t j = 0; j < sh; ++j)
        {
            b2_colour = bmp_get(b2, i, j);
            b1_colour = bmp_get(b1, dx + i, dx + j);
            
            bmp_put(dst, dx + i, dy + j, op(b1_colour, b2_colour)); 
        }
        
    return dst;
}


