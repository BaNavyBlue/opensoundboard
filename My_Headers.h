#ifndef MY_HEADERS_H
#define MY_HEADERS_H

#include <SDL2/SDL.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <istream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <map>
#include <vector>
#include <string>
#include <experimental/filesystem>
#include <stdlib.h>
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H 
#include FT_GLYPH_H
//#include "free_type_funcs.h"

#define TEN_MS 10000u
#define BYTES_S 176400u
#define TEN_MS_F 0.01f
#define TAN_35DEG 0.70020753820969f
#define SXTN_X_NINE 1.7777777777778f

const float SCALE_OPT_WIN = 0.5f;

struct point{
    int x;
    int y;
};

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

typedef enum WINDOW_TYPE{
    ESC_WIN = 0,
    BUTTON_WIN,
    MENU_WIN
};

typedef enum FIELD_TYPE{
    NEW = 0,
    SAVE,
    LOAD,
    EXIT,
    LABEL,
    SAMPLE,
    COLOR,
    REMOVE,
    ARROW_UP,
    ARROW_DN,
    NONE
};

//Screen dims
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;



#endif
