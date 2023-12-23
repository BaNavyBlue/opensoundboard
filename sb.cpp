/*
    This is where it all ends.

*/

//#include "My_Headers.h"
//#include "button.h"
#include "opt_window.h"
//#define _HAS_CXX17 1
#include <experimental/filesystem>
//#include "opt_field.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> 
#include "save_config.h"
//#include "shaderx.h"

// camera * UNUSED *
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  0.0f);
unsigned int texture1, texture2;

const int VH_SPACING = 14;
const int HORZ_OFFSET = 152;
const int VERT_OFFSET = 52;
const float DEPTH_NEAR = 0.1f;
const float DEPTH_FAR = 10.0f;



//Starts up SDL window
bool init();

//Initializes OGL rendering.
bool initGL();

//Input handler
void handleKeys( unsigned char key, int x, int y );

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

void RenderText(Shader *shader, std::string text, float x, float y, float scale, glm::vec3 color);
void render_text(Shader &shader, const char *text, float x, float y, float sx, float sy);
void setUpFont(const char *file);
void renderFontmetrics();

// Using the SDL thread struct for a simple counter.
int counting_thread(void* data);

unsigned int atlas_tex = 0;
int atlas_width = 0;
int atlas_height = 0;

struct character_info {
    float ax; // advance.x
    float ay; // advance.y

    float bw; // bitmap.width;
    float bh; // bitmap.rows;

    float bl; // bitmap_left;
    float bt; // bitmap_top;

    float tx; // x offset of glyph in texture coordinates
} c[128];

//The Window We be doing the doo
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

unsigned int VBO, VAO, VBO1, VAO1;

//Render Flag
extern bool gRenderQuad = true;
//Main loop flag
extern bool quit = false;
extern bool esc_button = false;
extern bool button_window = false;
extern int button_win_ID = -1;

std::map<char, Character> Characters;

int main(){

    //std::file
    //std::filesystem::directory_iterator d;
    std::vector<std::string> dir;
    for(auto& p: std::experimental::filesystem::directory_iterator("samples")){
        dir.push_back(p.path());
    }

    for(std::size_t i = 0; i < dir.size(); i++){
        printf("Filename: %s\n", dir[i].c_str());
    }
    
    //Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		

        // build and compile our shader program
        // ------------------------------------
        Shader ourShader("shaders/shader.vs", "shaders/shader.fs"); // you can name your shader files however you like
        //Not sure if will work out of this context, the Shader Struct

		//Event handler
		SDL_Event e;
		
		//Enable text input
		SDL_StartTextInput();

        // TTF Font stuff
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return -1;
        }

        
	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, "arial.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 128);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load Most Printable characters of ASCII set
        for (unsigned char c = 32; c < 255; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }





    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    Shader shader("shaders/text.vs", "shaders/text.fs");
    Shader myShader("shaders/my_text.vs", "shaders/my_text.fs");
    Shader window_shader("shaders/window_shader.vs","shaders/window_shader.fs");
    Shader fieldTextShader("shaders/field_text.vs", "shaders/field_text.fs");
    float h_spacing = ((float)VH_SPACING / (SCREEN_WIDTH / 2.0f));
    float v_spacing = ((float)VH_SPACING / (SCREEN_HEIGHT / 2.0f));// + 1.0f;// * 0.7232f;
    float horz_off_model = ((float)HORZ_OFFSET / (SCREEN_WIDTH / 2));
    float vert_off_model = ((float)VERT_OFFSET / (SCREEN_HEIGHT / 2 ));// + 1.0f;
    glm::vec2 position = glm::vec2(-1.0, 3.25f - (v_spacing)); // button screen center
    printf("horz_off: %f vert_off: %f\n", horz_off_model, vert_off_model);
    printf("h_spacing: %f, v_spacing: %f\n", h_spacing, v_spacing);
    
    // Get data For Threads initialized;
    CountThreadData data;

    std::vector<countID> ButtonID;
    data.ButtonID = &ButtonID;
    data.counterLock = SDL_CreateMutex();
    data.quit = &quit;
    data.text_shader = &shader;
    
    
    std::vector<BUTTON> button_vec;
    for(int i = 0; i < 60; i++){
        BUTTON button(&ourShader, &myShader, &Characters);
        button.counterLock = data.counterLock; // give address of mutex to buttons
        button.ButtonID = &ButtonID; // vector for counter thread

        if(i % 6 == 0){
            position.x = -5.45f + h_spacing;
            button.Button_Color = glm::vec3(0.3f, 0.0f,0.3f); // Purple
            if(i != 0){
                position.y -= 3*(vert_off_model + 2 * v_spacing);
            }
    
        }else if(i % 6 == 1){
            button.Button_Color = glm::vec3(0.0f, 0.0f, 0.3f); // Blue
        }else if(i % 6 == 2){
            button.Button_Color = glm::vec3(0.0f, 0.3f, 0.3f); // Teal/Cyan
        }else if(i % 6 == 3){
            button.Button_Color = glm::vec3(0.0f, 0.3f, 0.0f); // Green
        }else if(i % 6 == 4){
            button.Button_Color = glm::vec3(0.3f, 0.3f, 0.0f); // Yellow
        }else if(i % 6 == 5){
            button.Button_Color = glm::vec3(0.3f, 0.0f, 0.0f); // Red
        }
        

        //printf("Position.x %f, Position.y %f\n", position.x, position.y);

        
        button.model = glm::mat4(1.0f);
        button.view = glm::mat4(1.0f);
        button.projection = glm::mat4(1.0f);
 
        // Apply Transformations to Button Shader
        // (transformations are: scale happens first, then rotation, and then final translation happens; reversed order) 
        button.model = glm::scale(button.model, glm::vec3(0.2f));
        button.model = glm::translate(button.model, glm::vec3(position, 0.0f)); 
        button.view = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));  
     
        button.projection = glm::perspective(glm::radians(70.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, DEPTH_NEAR, DEPTH_FAR);
        button.Top_Left = button.projection*button.view*button.model*button.Top_Left;
        button.Bottom_Right = button.projection*button.view*button.model*button.Bottom_Right;
        
        glm::vec4 topL = button.Top_Left;
        glm::vec4 BotR = button.Bottom_Right;
        button.T_Left.x = (topL.x + 1.0f) * (SCREEN_WIDTH/2.0f);
        button.T_Left.y = (-topL.y + 1.0f)*SCREEN_HEIGHT / 2.0f;
        button.B_Right.x = (BotR.x + 1.0f)*SCREEN_WIDTH/2.0f;
        button.B_Right.y = (-BotR.y + 1.0f)*SCREEN_HEIGHT / 2.0f;
        button.init_button();
        button.Button_Window = new OPT_WINDOW(&window_shader, &fieldTextShader, &Characters, BUTTON_WIN);
        button.Button_Window->counterLock = data.counterLock;
        button.Button_Window->projection = button.projection;
        button.Button_Window->model = glm::mat4(1.0f);
        button.Button_Window->init_fields();//Field Type should be assigned now
        button_vec.push_back(button);

        position.x += 6*(horz_off_model + 2 * h_spacing);
    }
    // This is just a test of what will be button window
    OPT_WINDOW test(&window_shader, &fieldTextShader, &Characters, BUTTON_WIN);
    test.counterLock = data.counterLock;
    test.projection = glm::perspective(glm::radians(70.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, DEPTH_NEAR, DEPTH_FAR);
    test.model = glm::mat4(1.0f);
    for(int i = 0; i < 4; i++){
        printf("%f %f %f %f\n", test.model[i][0], test.model[i][1], test.model[i][2], test.model[i][3]);
    }
    printf("\n");

    test.model = glm::translate(test.model, glm::vec3(-0.8f, 0.125f, -1.0f));
    for(int i = 0; i < 4; i++){
        printf("%f %f %f %f\n", test.model[i][0], test.model[i][1], test.model[i][2], test.model[i][3]);
    }
    printf("\n");

    test.model = glm::scale(test.model, glm::vec3(SCALE_OPT_WIN));
    for(int i = 0; i < 4; i++){
        printf("%f %f %f %f\n", test.model[i][0], test.model[i][1], test.model[i][2], test.model[i][3]);
    }
    printf("\n");

    for(int i = 0; i < 4; i++){
        printf("%f %f %f %f\n", test.projection[i][0], test.projection[i][1], test.projection[i][2], test.projection[i][3]);
    }
    printf("\n");
    
    // need a bester way to do this wanted to try pointers but it wouldn't work glm::ret_pointer issue
    test.init_fields();
    test.TL_Corner = test.projection * test.model * test.TL_Corner; 
    test.BR_Corner = test.projection * test.model * test.BR_Corner;
    test.S_COORD_TL.x = (test.TL_Corner.x + 1.0f) * (SCREEN_WIDTH / 2.0f);
    test.S_COORD_TL.y = (-test.TL_Corner.y + 1.0f) * (SCREEN_HEIGHT / 2.0f);
    test.S_COORD_BR.x = (test.BR_Corner.x + 1.0f) * (SCREEN_WIDTH / 2.0f);
    test.S_COORD_BR.y = (-test.BR_Corner.y + 1.0f) * (SCREEN_HEIGHT / 2.0f);
    printf("TEST TL_w.x: %f TL_w.y: %f\n", test.TL_Corner.x, test.TL_Corner.y);
    printf("TEST BR_w.x: %f BR_w.y: %f\n", test.BR_Corner.x, test.BR_Corner.y);
    printf("TEST TL.x: %d TL.y: %d\n", test.S_COORD_TL.x, test.S_COORD_TL.y);
    printf("TEST BR.x: %d BR.y: %d\n", test.S_COORD_BR.x, test.S_COORD_BR.y);
    

    // This will probably stay as the escape button window;
    OPT_WINDOW esc_window(&window_shader, &fieldTextShader, &Characters, ESC_WIN);

    // Basic Rough Configuration for ESC Window
    esc_window.counterLock = data.counterLock;
    esc_window.projection =  glm::perspective(glm::radians(70.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, DEPTH_NEAR, DEPTH_FAR);
    esc_window.model = glm::mat4(1.0f);
    esc_window.model = glm::translate(esc_window.model, glm::vec3(0.0f,0.0f,-1.0f));
    esc_window.model = glm::scale(esc_window.model, glm::vec3(SCALE_OPT_WIN));

    // need to find a better way to do this tried with pointers but glm::value_ptr(model)
    // was casuing bad things
    esc_window.init_fields();

    esc_window.TL_Corner = esc_window.projection * esc_window.model * esc_window.TL_Corner; 
    esc_window.BR_Corner = esc_window.projection * esc_window.model * esc_window.BR_Corner;
    esc_window.S_COORD_TL.x = (esc_window.TL_Corner.x + 1.0f) * (SCREEN_WIDTH / 2.0f);
    esc_window.S_COORD_TL.y = (-esc_window.TL_Corner.y + 1.0f) * (SCREEN_HEIGHT / 2.0f);
    esc_window.S_COORD_BR.x = (esc_window.BR_Corner.x + 1.0f) * (SCREEN_WIDTH / 2.0f);
    esc_window.S_COORD_BR.y = (-esc_window.BR_Corner.y + 1.0f) * (SCREEN_HEIGHT / 2.0f);
    // Verify Bounding Corner Coordinates on screen
    printf("TL_w.x: %f TL_w.y: %f\n", esc_window.TL_Corner.x, esc_window.TL_Corner.y);
    printf("BR_w.x: %f BR_w.y: %f\n", esc_window.BR_Corner.x, esc_window.BR_Corner.y);
    printf("TL.x: %d TL.y: %d\n", esc_window.S_COORD_TL.x, esc_window.S_COORD_TL.y);
    printf("BR.x: %d BR.y: %d\n", esc_window.S_COORD_BR.x, esc_window.S_COORD_BR.y);
    
    glm::vec4 coordCheck = glm::vec4(-0.5f, 1.0f, 1.0f, 1.0f);

    SDL_Thread* threadID = SDL_CreateThread( counting_thread, "MagicThread", (void*)&data );
    
    printf("Loading Samples\n");
    button_vec[0].samplePath = "samples/low.wav";
    button_vec[0].sample = Mix_LoadWAV(button_vec[0].samplePath.c_str());
    button_vec[0].label = "low.wav";
    button_vec[0].Button_Window->fields[0].label = button_vec[0].label;
    button_vec[0].sampleDuration = ((float)button_vec[0].sample->alen) / BYTES_S;
    
    if( button_vec[0].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
	}
    button_vec[1].samplePath = "samples/medium.wav";
    button_vec[1].sample = Mix_LoadWAV(button_vec[1].samplePath.c_str());
    button_vec[1].label = "medium.wav";
    button_vec[1].Button_Window->fields[0].label = button_vec[1].label;
    button_vec[1].sampleDuration = ((float)button_vec[1].sample->alen) / BYTES_S;
    if( button_vec[1].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
	}
    button_vec[2].samplePath = "samples/high.wav";
    button_vec[2].sample = Mix_LoadWAV(button_vec[2].samplePath.c_str());
    button_vec[2].label = "high.wav";
    button_vec[2].Button_Window->fields[0].label = button_vec[2].label;
    button_vec[2].sampleDuration = ((float)button_vec[2].sample->alen) / BYTES_S;
    if( button_vec[2].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
	}
    button_vec[3].samplePath = "samples/GMILF.wav";
    button_vec[3].sample = Mix_LoadWAV(button_vec[3].samplePath.c_str());
    button_vec[3].label = "GMILF.wav";
    button_vec[3].Button_Window->fields[0].label = button_vec[3].label;
    button_vec[3].sampleDuration = ((float)button_vec[3].sample->alen) / BYTES_S;
    if( button_vec[3].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
	}
    button_vec[4].samplePath = "samples/WhyDidYouLeaveMe.wav";
    button_vec[4].sample = Mix_LoadWAV(button_vec[4].samplePath.c_str());
    button_vec[4].label = "WhyDidYou";
    button_vec[4].Button_Window->fields[0].label = button_vec[4].label;
    button_vec[4].sampleDuration = ((float)button_vec[4].sample->alen) / BYTES_S;
    if( button_vec[4].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
    
	}
    button_vec[5].samplePath = "samples/cancer.wav";
    button_vec[5].sample = Mix_LoadWAV(button_vec[5].samplePath.c_str());
    button_vec[5].label = "cancer.wav";
    button_vec[5].Button_Window->fields[0].label = button_vec[5].label;
    button_vec[5].sampleDuration = ((float)button_vec[5].sample->alen) / BYTES_S;
    if( button_vec[5].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
	}
    button_vec[6].samplePath = "samples/HamburgerPrease.wav";
    button_vec[6].sample = Mix_LoadWAV(button_vec[6].samplePath.c_str());
    button_vec[6].label = "HmbrgerPreas";
    button_vec[6].Button_Window->fields[0].label = button_vec[6].label;
    button_vec[6].sampleDuration = ((float)button_vec[6].sample->alen) / BYTES_S;
    if( button_vec[6].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
	}
    button_vec[7].samplePath = "samples/WowIE.wav";
    button_vec[7].sample = Mix_LoadWAV(button_vec[7].samplePath.c_str());
    button_vec[7].label = "WowIE";
    button_vec[7].Button_Window->fields[0].label = button_vec[7].label;
    button_vec[7].sampleDuration = ((float)button_vec[7].sample->alen) / BYTES_S;
    if( button_vec[7].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
	}
    button_vec[8].samplePath = "samples/NoticeMe.wav";
    button_vec[8].sample = Mix_LoadWAV(button_vec[8].samplePath.c_str());
    button_vec[8].label = "NoticeMe";
    button_vec[8].Button_Window->fields[0].label = button_vec[8].label;
    button_vec[8].sampleDuration = ((float)button_vec[8].sample->alen) / BYTES_S;
    if( button_vec[8].sample == NULL )
	{
		printf( "Failed to load! SDL_mixer Error: %s\n", Mix_GetError() );
		//success = false;
	}
    printf("done loating samples\n");

    glm::mat4 projectionTTF = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionTTF));

    for(int i = 0; i < 3; i++){
        printf("ortho proj x: %f y: %f z: %f w: %f\n", projectionTTF[i][0], projectionTTF[i][1], projectionTTF[i][2], projectionTTF[i][3] );
    }
    
    
    // projection matrix?
    glm::mat4 model = glm::mat4(1.0f);
    for(int i = 0; i < 3; i++){
        printf("model proj x: %f y: %f z: %f w: %f\n", model[i][0], model[i][1], model[i][2], model[i][3] );
    }
    glm::mat4 trans = glm::mat4(1.0f);


    // The transform declaration
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    for(int i = 0; i < 3; i++){
        printf("pers proj x: %f y: %f z: %f w: %f\n", projection[i][0], projection[i][1], projection[i][2], projection[i][3] );
    }

    ourShader.setMat4("projection", projection);
    ourShader.use();
    glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // camera/view transformation
    ourShader.use();
    glm::mat4 view = glm::mat4(1.0f);

    for(int i = 0; i < 3; i++){
        printf("view proj x: %f y: %f z: %f w: %f\n", view[i][0], view[i][1], view[i][2], view[i][3] );
    }
    ourShader.setMat4("view", view);

    ourShader.use();
    int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    int x = 0, y = 0;
    char thisword[9] = { (char)181, 'B', 'i', 'z', 'q', 'i', 't','!', '\0'};
    button_vec[0].label = thisword;
    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0)
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            //Handle button option window
            if(button_window){
                //Mouse is left of the button
                if( x < button_vec[button_win_ID].Button_Window->S_COORD_TL.x )
                {
                    button_window = false;
                }
                //Mouse is right of the button
                else if( x > button_vec[button_win_ID].Button_Window->S_COORD_BR.x )
                {
                    button_window = false;
                }
                //Mouse above the button
                else if( y < button_vec[button_win_ID].Button_Window->S_COORD_TL.y )
                {
                    button_window = false;
                }
                //Mouse below the button
                else if( y > button_vec[button_win_ID].Button_Window->S_COORD_BR.y )
                {
                    button_window = false;
                } 
                else
                {
                    // check button window fields
                    for(std::size_t i = 0; i < button_vec[button_win_ID].Button_Window->fields.size(); i++){
                        button_vec[button_win_ID].Button_Window->fields[i].handleEvent(&e, x, y);
                    }
                }
                if(!button_window){
                    button_win_ID = -1;
                }
            }

            //Handle Escape key for menu
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    esc_button = !esc_button;
                }
            }
            if(esc_button){
                for(std::size_t i = 0; i < esc_window.fields.size(); i++){
                    esc_window.fields[i].handleEvent(&e, x, y);
                }
            }
            //Handle keypress with current mouse position
            else if( e.type == SDL_TEXTINPUT )
            {
                
                SDL_GetMouseState( &x, &y );
                handleKeys( e.text.text[ 0 ], x, y );
            }
            for(int i = 0; i < 60; i++){
                button_vec[i].handleEvent(&e, x, y);
            }
        }

        // deluminate playing button after finished
        if(data.finished){
            button_vec[data.clearID].handleEvent(data.finished);
            data.clearID = 0;
            data.finished = false;            
        }

        //Render quad
        if(gRenderQuad)
        {
            // pass projection matrix to shader (note that in this case it could change every frame)
            
            //Set Back Ground Color and Clear set depth buffer bit
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
            glEnable(GL_DEPTH_TEST);
            
            for(int i = 0; i < 60; i++){
                button_vec[i].draw_button();            
            }
            //glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            if(esc_button){
                esc_window.draw_window();
            }
            if(button_window){
                button_vec[button_win_ID].Button_Window->draw_window();
            }
            //test.draw_window();
            ourShader.use();

            char mouse_info[256];

            sprintf(mouse_info, "x-pos: %d y-pos: %d", x, y);
           
            RenderText(&shader, mouse_info, 25.0f, 25.0f, 0.25f, glm::vec3(0.5, 0.8f, 0.2f));
            char time_str[256];
            SDL_LockMutex(data.counterLock);
            sprintf(time_str, "Time Elapsed:\n %d:%02d::%02d", data.minutes, data.seconds, data.Hundredths);
            SDL_UnlockMutex(data.counterLock);
            RenderText(&shader, time_str, 1080.0f, 14.0f, 0.125f, glm::vec3(0.3, 0.7f, 0.9f));


            gRenderQuad = false;
        }
        //render();

        //Update screen
        SDL_GL_SwapWindow( gWindow );
    }
    //FT_Done_Face(face);
    //Destroy the mutex
    SDL_WaitThread(threadID, NULL);
    //SDL_WaitThread(txtThreadID, NULL);
    SDL_DestroyMutex( data.counterLock );
    data.counterLock = NULL;

    //Disable text input
    SDL_StopTextInput();
	}
    
	//Free resources and close SDL
	close();

	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		//Create window
		gWindow = SDL_CreateWindow( "Open Sound Board", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize GLEW
				//glewExperimental = GL_TRUE; 
				//GLenum glewError = glewInit();
				//if( glewError != GLEW_OK )
				//{
				//	printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
				//}

                // glad: load all OpenGL function pointers
                // ---------------------------------------
                if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
                {
                    std::cout << "Failed to initialize GLAD" << std::endl;
                    return -1;
                }

				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL
				if( !initGL() )
				{
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}

                 //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL()
{
	//Success flag
	bool success = true;

    
 	
	return success;
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	if( key == 'q' )
	{
		gRenderQuad = !gRenderQuad;
	}
    if(key == SDLK_ESCAPE){
       esc_button = !esc_button;
    }
}

void update()
{
	//No per frame update needed
}

void render()
{
	//Clear color buffer
	//glClear( GL_COLOR_BUFFER_BIT );
	
	//Render quad
	
}

void close()
{
	//Deallocate program


	//Destroy window	
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
    Mix_Quit();
	//Quit SDL subsystems
	SDL_Quit();
}

void printProgramLog( GLuint program )
{
	//Make sure name is shader
	if( glIsProgram( program ) )
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		//Get info string length
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		char* infoLog = new char[ maxLength ];
		
		//Get info log
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}
		
		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a program\n", program );
	}
}

void printShaderLog( GLuint shader )
{
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		char* infoLog = new char[ maxLength ];
		
		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a shader\n", shader );
	}
}

// render line of text
// -------------------

void RenderText(Shader *shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    //printf("%s\n", text.c_str());
    // Get those buffers all buff
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader->use();
    glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void render_text(Shader &shader, const char *text, float x, float y, float sx, float sy) {
    struct point {
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    } coords[6 * strlen(text)];
    


    
    int n = 0;

  for(const char *p = text; *p; p++) { 
    float x2 =  x + c[*p].bl * sx;
    float y2 = -y - c[*p].bt * sy;
    float w = c[*p].bw * sx;
    float h = c[*p].bh * sy;

    /* Advance the cursor to the start of the next character */
    x += c[*p].ax * sx;
    y += c[*p].ay * sy;

    /* Skip glyphs that have no pixels */
    if(!w || !h)
      continue;

    coords[n++] = (point){x2,     -y2    , c[*p].tx,                                            0};
    coords[n++] = (point){x2 + w, -y2    , c[*p].tx + c[*p].bw / atlas_width,   0};
    coords[n++] = (point){x2,     -y2 - h, c[*p].tx,                                          c[*p].bh / atlas_height}; //remember: each glyph occupies a different amount of vertical space
    coords[n++] = (point){x2 + w, -y2    , c[*p].tx + c[*p].bw / atlas_width,   0};
    coords[n++] = (point){x2,     -y2 - h, c[*p].tx,                                          c[*p].bh / atlas_height};
    coords[n++] = (point){x2 + w, -y2 - h, c[*p].tx + c[*p].bw / atlas_width,                 c[*p].bh / atlas_height};
  }
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
  
  glBindTexture(GL_TEXTURE_2D, texture2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);
  glDrawArrays(GL_TRIANGLES, 0, n);
}

int counting_thread(void* data){
    CountThreadData* thd_data = (CountThreadData*)data;
    //unsigned int ticks = 0;
    while(!*(thd_data->quit)){
        usleep(TEN_MS); //Sleeps for 1/100th of a second  For counting
        SDL_LockMutex(thd_data->counterLock);
            thd_data->Hundredths++;
            if(thd_data->Hundredths == 100){
                thd_data->seconds++;
                thd_data->Hundredths = 0;
            }
            if(thd_data->seconds == 60){
                thd_data->minutes++;
                thd_data->seconds = 0;
            }
            if(thd_data->ButtonID->size()){
                //for(auto it = thd_data->ButtonID->begin();it != thd_data->ButtonID->end() ; it++)
                for(std::size_t i = 0; i < thd_data->ButtonID->size(); i++){
                     auto it = thd_data->ButtonID->begin() + i;
                     //printf("timeRemain: ;%f\n", *it->timeRemain);
                     *it->timeRemain -= TEN_MS_F;
                     if(*it->timeRemain < 0.01f){
                         *it->timeRemain = 0.0f;
                         thd_data->clearID = *it->ID;
                         thd_data->ButtonID->erase(it);
                         thd_data->finished = true;
                         //gRenderQuad = true;
                     }
                }
            }
        SDL_UnlockMutex(thd_data->counterLock);
        if(thd_data->Hundredths % 5 == 0){
           gRenderQuad = true;
        }
    }
    std::cout << "Exiting count thread" << std::endl;
    return 0;
}

