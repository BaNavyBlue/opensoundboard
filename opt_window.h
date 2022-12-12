#ifndef OPT_WINDOW_H
#define OPT_WINDOW_H

#include "My_Headers.h"
#include "opt_field.h"



class OPT_WINDOW{
    public:
        // Constructor
        OPT_WINDOW();
        OPT_WINDOW(Shader *win_shader, Shader *text, std::map<char, Character> *Chars, WINDOW_TYPE TYPE);
        //~OPT_WINDOW();
        WINDOW_TYPE W_TYPE;
        Shader *shader;
        Shader *text_shader;
        SDL_mutex *counterLock;
        std::map<char, Character> *Characters;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::vec4 TL_Corner = glm::vec4(-0.5f,1.0f, 1.0f, 1.0f);// Base Value
        glm::vec4 BR_Corner = glm::vec4(0.5f,-1.0f, 1.0f, 1.0f);// Base Value
        point S_COORD_TL;
        point S_COORD_BR;
        std::vector<class OPT_FIELD> fields;
        void draw_window();
        //void init_window();
        unsigned int VAO;
        void init_fields();
        void handleEvent( SDL_Event* e, int &x, int &y );
        void set_field_values();
        void calc_bound_box();
    private:
        void init_window_body();
        void draw_window_body();
        void init_menu_window_body();
        void init_window_rows();
        void draw_window_rows();
};


#endif