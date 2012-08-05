#include <stdio.h>
#include <string.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "eikon.h"

static void error(const char *err)
{
    fprintf(stderr, "%s\n", err);
}

unsigned font_faces(const char *filename)
{
    font *font = malloc(sizeof(struct tagFont));
    
    if (!font) return 0;
    
    FT_Error err;
    
    err = FT_Init_FreeType(&font->lib);
    
    if (err)
    {
        return 0;
    }
    
    err = FT_New_Face(font->lib, filename, -1, &font->face);
    
    if (err == 0)
    {   // font format is recognized
    
        unsigned qty = font->face->num_faces;
        FT_Done_Face(font->face);    
        FT_Done_FreeType(font->lib);
        free(font);
        
        return qty;
    }
    
    FT_Done_Face(font->face);    
    FT_Done_FreeType(font->lib);
    free(font);
    
    return 0;
}

font *font_init(const char *filename, int face)
{
    font *font = malloc(sizeof(struct tagFont));
    
    if (!font) return NULL;
    
    FT_Error err;
    
    err = FT_Init_FreeType(&font->lib);
    
    if (err)
    {
        error("Error initializing the FreeType library.");
        return NULL;
    }
    
    err = FT_New_Face(font->lib, filename, face, &font->face);
    
    if (err == FT_Err_Unknown_File_Format)
    {
        error("The font file could be opened, but file format not supported.");
        FT_Done_FreeType(font->lib);
        free(font);
        return NULL;
    }
    else if (err)
    {
        error("File could not be opened.");
        FT_Done_FreeType(font->lib);
        free(font);
        return NULL;
    }   
    
    return font;
}

void font_free(font *font)
{
    if (font)
    {
        FT_Done_Face(font->face);
        FT_Done_FreeType(font->lib);
        free(font);
    }
}

unsigned font_numglyphs(font *font)
{
    if (font)
        return font->face->num_glyphs;

    error("font_numglyphs called without font");
    abort();

}

bool font_scalable(font *font)
{
    if (font)
        return font->face->face_flags & FT_FACE_FLAG_SCALABLE;
    
    error("font_scalable called without font");
    abort();
} 

bool font_bitmap(font *font)
{
   if (font)
        return font->face->face_flags & FT_FACE_FLAG_FIXED_SIZES;
    
    error("font_bitmap called without font");
    abort();
}

bool font_monospace(font *font)
{
    if (font)
        return font->face->face_flags & FT_FACE_FLAG_FIXED_WIDTH;
    
    error("font_scalable called without font");
    abort();
}

// must be scalable
unsigned font_units_per_em(font *font)
{
    if (font)
        return font->face->units_per_EM;
    
    error("font_units_per_em called without font");
    abort();
}

unsigned font_bitmap_strikes(font *font)
{
    if (font)
        return font->face->num_fixed_sizes;
    
    error("font_bitmap_strikes called without font");
    abort();
}

// assumes sizes has font_bitmap_strikes elements
void font_bitmap_strike_sizes(font *font, int **sizes)
{
    if (font && sizes)
    {
        for (int i = 0; i < font->face->num_fixed_sizes; ++i)
        {
            sizes[i][0] = font->face->available_sizes[i].width;
            sizes[i][1] = font->face->available_sizes[i].height;
        }
        
        return;
    }
    
    error("font_bitmap_strike_sizes called without font");
    abort();
    
}

/*
int main()
{
   FT_Error error;
   FT_Library library;

   error = FT_Init_FreeType(&library);
   if (error)
   {
      fprintf(stderr, "Error initializing the free type library.\n");
      exit(1);
   }

   FT_Face face;

   error = FT_New_Face(library,
                       "CPMono_v07_Light.otf",
                       0,
                       &face);

   if (error == FT_Err_Unknown_File_Format)
   {
      fprintf(stderr,
              "The font file could be opened, but file format not supported.\n");
      exit(1);
   }
   else if (error)
   {
   
      fprintf(stderr,
              "File could not be opened.\n");
      exit(1);
   }

   printf("Number of faces: %ld\n", face->num_faces);
   printf("Number of Glyphs: %ld\n", face->num_glyphs);

   error = FT_Set_Char_Size(face, 0, 16*64, 300, 300);

   if (error)
   {
      fprintf(stderr,
              "Character size could not be set.\n");
      exit(1);
   }

   FT_Uint glyph_index = FT_Get_Char_Index(face, 'a');

   error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

   if (error)
   {
      fprintf(stderr,
              "Glyph Index could not be retrieved.\n");
      exit(1);
   }

   error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

   if (error)
   {
      fprintf(stderr,
              "Could not render glyph.\n");
      exit(1);
   }

   

   return 0;
   
   

   
}
*/