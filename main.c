
#include <stdbool.h>

#include "bitmap.h"

int main()
{
    Bitmap b;
    
    b = createBitmap(640, 480);
    
    fillBitmap(b, Pixel(255, 0, 0, 255));
    
    saveBitmap(b, "test.png");
    deleteBitmap(b);
    
}