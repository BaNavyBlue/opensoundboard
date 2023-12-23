#ifndef BUTTON_H
#define BUTTON_H
#include "My_Headers.h"
#include "opt_window.h"




// pass time data to timer thread
struct countID{
    int *ID;
    float *timeRemain;
};

//For timer thread
struct CountThreadData{
    unsigned int Hundredths = 0;
    unsigned int seconds = 0;
    unsigned int minutes = 0;
    bool* quit;
    bool finished = false;
    std::vector<countID> *ButtonID;
    SDL_mutex *counterLock;
    Shader *text_shader;
};

struct TextInThreadData{
    bool* quit;
};


//Render Flag
extern bool gRenderQuad;
extern bool button_window;
extern bool esc_button;
extern int button_win_ID;

class BUTTON{
    public:
        
        std::string label = "Default";
        std::string samplePath;
        Shader *Button_Body;
        Shader *Button_Text;
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 Button_Color = glm::vec3(0.3f, 0.0f, 0.3f); // Purples
        glm::vec4 Top_Left = glm::vec4(-0.75f, 0.25f, -0.1f, 1.0f);
        glm::vec4 Bottom_Right = glm::vec4(0.75f, -0.25f, -0.1f, 1.0f);
        point T_Left, B_Right; // integers bounding boxes for mouse over events
        Mix_Chunk *sample = NULL;
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float ring = 0.0f;
        float press = 0.0f;
        float playing = 0.0f;
        unsigned int VAO_Body, VAO_BACK, VAOTxt, VBOTxt;
        std::vector<unsigned int> VAOVect;
        std::map<char, Character> *Characters;
        std::vector<countID> *ButtonID;
        OPT_WINDOW *Button_Window;
        bool prev_visit = false;
        int ID;
        float sampleDuration = 0.0f;
        float timeRemain = 0.0f;
        SDL_mutex *counterLock;

        // Constructor
        BUTTON();
        BUTTON(Shader *Body, Shader *Text, std::map<char, Character> *Font_Map);
        
        void init_button();
        void draw_button(/*glm::mat4 position*/);  
        void handleEvent( SDL_Event* e , int &x, int &y );

    private:
        void draw_back();
        void draw_text();
        void draw_body();
        void init_back();
        void init_text();
        void init_body();
    
};





#endif