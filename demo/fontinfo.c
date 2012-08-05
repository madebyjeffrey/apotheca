

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
    check(argc == 2, "No font specified.\n");

    unsigned faces = font_faces(argv[1]);
    
    printf("Report for font file %s\n", argv[1]);
    printf("Number of faces: %d\n", faces);    
    
    for (int i = 0; i < faces; ++i)
    {
        font *font = font_init(argv[1], i);
        
        check(font, "Unexpected end after init font.");
        
        printf("Face %d:\n", i);
        
        printf("Number of glyphs: %d\n", font_numglyphs(font));
        printf("Scalable? %s\n", font_scalable(font) ? "yes" : "no");
        printf("Has bitmap strikes? %s\n", font_bitmap(font) ? "yes" : "no");
        printf("Monospace? %s\n", font_monospace(font) ? "yes" : "no");
        if (font_scalable(font))
        {
            printf("Units per EM: %d\n", font_units_per_em(font));
        }
        
        if (font_bitmap(font))
        {
            unsigned strikes = font_bitmap_strikes(font);
            printf("Number of bitmap strikes: %d\n", strikes);
//            int sizes[strikes][2];
            int **sizes;
            sizes = malloc(2 * sizeof(int*));
            sizes[0] = malloc(sizeof(int)*strikes);
            sizes[1] = malloc(sizeof(int)*strikes);
            
//            sizes = (unsigned**)malloc(strikes * sizeof(unsigned)*2);
            font_bitmap_strike_sizes(font, sizes);
            printf("Sizes: ");
            int c = 0;
            for (int p = 0; p < strikes; ++p)
            {
                printf("(%d x %d) ", sizes[p][0], sizes[p][1]);
                c++;
                if (c == 5)
                {
                    printf("\n");
                    c = 0;
                }
                
                
            }
            free(sizes[0]);
            free(sizes[1]);
            free(sizes);
        }
        
        font_free(font);
    }
}

