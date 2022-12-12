#ifndef FREE_TYPE_FUNCS_H
#define FREE_TYPE_FUNCS_H


#include <ft2build.h>
#include <glad/glad.h>
#include FT_FREETYPE_H 
#include FT_GLYPH_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glut.h>



typedef struct  TGlyph_
{
  FT_UInt    index;  /* glyph index                  */
  FT_Vector  pos;    /* glyph origin on the baseline */
  FT_Glyph   image;  /* glyph image                  */

} TGlyph, *PGlyph;

class Glyph_String{
     private:
         FT_UInt num_glyphs = 0;
         void  compute_string_bbox( FT_BBox  *abbox );

};
#endif