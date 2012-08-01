
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bitmap.h"

void check(bool a, char *err)
{
    if (!a) 
    {
        fprintf(stderr, "%s\n", err);
        exit(1);
    }
}

int main(int argc, char**argv)
{
    Bitmap b;
    
/*    b = createBitmap(640, 480);
    
    check(b, "Could not create the bitmap");
    
    bool result;
    result = fillBitmap(b, Pixel(0, 0, 255, 255));
    
    check(result, "fillBitmap failed");
    
    result = saveBitmap(b, "test.png");

    check(result, "write test failed");
    
    result = enlargeBitmap(b, 800, 600, Pixel(0, 255, 0, 255));
    
    check(result, "enlargeBitmap failed");

    result = saveBitmap(b, "test2.png");
    check(result, "write test2 failed");
    
    Bitmap c = createBitmap(180, 180);
    
    check(c, "second bitmap not created");    
    
    result = fillBitmap(c, Pixel(255, 0, 0, 255));
    check(result, "second fill no work");

    result = blitBitmap(b, 10, 10, 
                        c, 0, 0, 180, 180);
                        
    check(result, "blit no work");;
    
    result = saveBitmap(b, "test3.png");
    check(result, "write test3 failed");

*/

    check(argc > 3, "Not enough arguments.");
    bool x;
    
    x = loadBitmap(&b, argv[1]);
    
    check(x, "Could not load bitmap.");
    
    int cx = atoi(argv[2]);
    int cy = atoi(argv[3]);
    
    fprintf(stderr, "Resize to %d x %d\n", cx, cy);
    
    x = resizeBitmap(b, cx, cy);
    check(x, "Could not resize bitmap");

    x = saveBitmap(b, "test6.png");
    
    check(x, "Could not save bitmap.");
    
    deleteBitmap(b);
    /*
    b = createBitmap(640, 480);
    
    for (int i = 0; i < 640; i+=4)
    {
        for (int j = 0; j < 480; ++j)
        {
            putPixel(b, i, j, Pixel(255, 0, 0, 255));
            putPixel(b, i+1, j, Pixel(0, 255, 0, 255));
            putPixel(b, i+2, j, Pixel(0, 0, 255, 255));
            putPixel(b, i+3, j, Pixel(255, 255, 255, 255));
        }
    }
    
    x = saveBitmap(b, "test5.png");
    
    check(x, "Could not save new bitmap");
    */
    deleteBitmap(b);
}