
#include <stdio.h>
#include <stdbool.h>

#include "bitmap.h"

int main()
{
    Bitmap b;
    
    b = createBitmap(640, 480);
    
    if (!b) 
    {
        fprintf(stderr, "Could not create the bitmap");
        return 1;
    }
    
    bool result;
    result = fillBitmap(b, Pixel(255, 0, 0, 255));
    
    if (!result) 
    {
        fprintf(stderr, "fillBitmap failed");
        return 1;
    }
    
    result = saveBitmap(b, "test.png");

    if (!result) 
    {
        fprintf(stderr, "write test failed");
        return 1;
    }

    
    result = resizeBitmap(b, 800, 600, Pixel(0, 255, 0, 255));
    
    if (!result) 
    {
        fprintf(stderr, "resizeBitmap failed");
        return 1;
    }

    
    result = saveBitmap(b, "test2.png");
    
    if (!result) 
    {
        fprintf(stderr, "write test2 failed");
        return 1;
    }

    
    deleteBitmap(b);
    
}