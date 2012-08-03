#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define PNG_DEBUG 4
#include "png.h"

#include "eikon.h"


pixel_t bmp_sample_nearestNeighbour(bmp *src, float x, float y)
{
    float dummy;
    float w = modff(x, &dummy);
    float h = modff(y, &dummy);
    
    int quad = (w < 0.5 ? (h < 0.5 ? 0 : 2)
                        : (h < 0.5 ? 1 : 3));
              
    switch (quad)
    {
        case 0:
            return bmp_get(src, floorl(x), floorl(y));
        case 1:
            return bmp_get(src, ceill(x), floorl(y));
        case 2:
            return bmp_get(src, floorl(x), ceill(y));
//        case 3: 
        default:
            return bmp_get(src, ceill(x), ceill(y));
    }

}

// Bilinear Sample
pixel_t bmp_sample_bilinear(bmp *src, float x, float y)
{
    pixel_t A, B, C, D;
    
    A = bmp_get(src, floorl(x), floorl(y));
    B = bmp_get(src, ceill(x), floorl(y));
    C = bmp_get(src, floorl(x), ceill(y));
    D = bmp_get(src, ceill(x), ceill(y));

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
    return Pixel(pR, pG, pB, pA);
}

bmp *bmp_resize(bmp *src, size_t cx, size_t cy, sampler s)
{
    if (!src) return false;
    if (!s) return false;
    
    bmp *dst = bmp_init(cx, cy);
    
    if (!dst) return false;
    
    pixel_t colour;
    
    for (size_t i = 0; i < cx; ++i)
    {
        for (size_t j = 0; j < cy; ++j)
        {
            colour = s(src, i * ((float)src->width / cx),
                              j * ((float)src->height / cy));
            bmp_put(dst, i, j, colour);
        }
    }
    
    return dst;
}

