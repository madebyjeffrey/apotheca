
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <eikon/eikon.h>

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
    bmp *b;
    
    b = bmp_init(640, 480);

    check(b, "Could not create the bitmap");

    bmp *fill = bmp_fill(b, Pixel(255, 0, 0, 255));

    check(fill, "Could not fill bitmap");

    bmp_write(fill, "filled.png");

    bmp_free(b);
    bmp_free(fill);
}
