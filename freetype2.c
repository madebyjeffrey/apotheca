#include <stdio.h>
#include <string.h>

#include <ft2build.h>
#include FT_FREETYPE_H



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
