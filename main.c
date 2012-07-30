
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

int main()
{
    Bitmap b;
    
    b = createBitmap(640, 480);
    
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

    


    
    deleteBitmap(b);
    
}