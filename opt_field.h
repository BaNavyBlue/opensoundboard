/* This is the option field handeling code.*/

#ifndef OPT_FIELD_H
#define OPT_FIELD_H
#include "My_Headers.h"
#include "opt_window.h"

extern bool gRenderQuad;
extern bool quit;

class OPT_FIELD{
    public:
        // Bounding boxes for mouse over events
        OPT_FIELD();
        OPT_FIELD(Shader *field_shader, Shader *text, float xoff, float yoff, int ID, std::map<char, Character> *Chars, WINDOW_TYPE W_TYPE);
        std::map<char, Character> *Characters;
        std::string label = "gMy Penjs q!";
        int FIELD_ID = 0;
        FIELD_TYPE TYPE;
        Shader *shader;
        Shader *text_shader;
        float y_off = 0.2375f;
        glm::mat4 model;
        glm::mat4 projection;
        float MOE = 0.0f; // Mouse over Event To change back ground "brightness"
        glm::vec3 color_text;
        //glm::mat4 view = glm::mat4(1.0f);
        unsigned int VAO, VBO, VAOTxt, VBOTxt;
        glm::vec4 TL;
        glm::vec4 BR;
        point TL_CORNER;
        point BR_CORNER;
        bool prev_visit = false;
        void draw_field();
        void draw_text(float x, float y, float z, glm::vec3 color);   
        void init_field();
        void handleEvent( SDL_Event* e, int &x, int &y );
    private:
        void bounding_box();
        void init_text(float x, float y, float z, glm::vec3 color); 
        
        

};

#endif