#include "button.h"

static int b_IDCnt = 0;

/*float *BUTTON::verticies(){
    return &(vectors);

}*/

BUTTON::BUTTON(){

}

BUTTON::BUTTON(Shader *Body, Shader *Text, std::map<char, Character> *Font_Map){
    // Give me all the pointers
    Button_Body = Body;
    Button_Text = Text;
    Characters = Font_Map;
    ID = b_IDCnt++;
    char name[32];
    sprintf(name, "Unused %d", ID + 1);
    label = name;
}

void BUTTON::init_button()
{
    init_back();
    init_text();
    init_body();  
}

void BUTTON::draw_button(/*glm::mat4 position*/){
    draw_back();
    draw_text();
    draw_body();
}

void BUTTON::init_back(){

    // Merely for special effects with the color helps the button look jewel like
    if( Button_Color.x > 0.0f){
        r = Button_Color.x * 0.5f;
    } else {
        r = 0.0f;
    }

    if( Button_Color.y > 0.0f){
        g = Button_Color.y * 0.5f;
    } else {
        g = 0.0f;
    }

    if( Button_Color.z > 0.0f){
        b = Button_Color.z * 0.5f;
    } else {
        b = 0.0f;
    }
    
    // Defines the Back of the Button Must be drawn first
    float back[54] = {
        -0.75f, -0.25f, -0.1f + press,  Button_Color.x + playing, Button_Color.y + playing, Button_Color.z + playing, 1.0,  0.0f, 0.0f, // Back
         0.75f, -0.25f, -0.1f + press,  Button_Color.x + r + playing, Button_Color.y + g + playing, Button_Color.z + b + playing, 1.0,  0.0f, 0.0f,
         0.75f,  0.25f, -0.1f + press,  Button_Color.x + r + playing, Button_Color.y + g + playing, Button_Color.z + b + playing, 1.0,  0.0f, 0.0f,
         0.75f,  0.25f, -0.1f + press,  Button_Color.x + r + playing, Button_Color.y + g + playing, Button_Color.z + b + playing, 1.0,  0.0f, 0.0f,
        -0.75f,  0.25f, -0.1f + press,  Button_Color.x + playing, Button_Color.y + playing, Button_Color.z + playing, 1.0,  0.0f, 0.0f,
        -0.75f, -0.25f, -0.1f + press,  Button_Color.x + playing, Button_Color.y + playing, Button_Color.z + playing, 1.0,  0.0f, 0.0f,
    };
    
    unsigned int VBO;
    // Use my button Shaders
    Button_Body->use();
    unsigned int modelLoc = glGetUniformLocation(Button_Body->ID, "model");
    unsigned int viewLoc  = glGetUniformLocation(Button_Body->ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    Button_Body->setMat4("projection", projection);
    glGenVertexArrays(1, &this->VAO_BACK);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(this->VAO_BACK);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(back), back, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);    
}

void BUTTON::draw_back()
{
    // Use my button Shaders
    Button_Body->use();
    unsigned int modelLoc = glGetUniformLocation(Button_Body->ID, "model");
    unsigned int viewLoc  = glGetUniformLocation(Button_Body->ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    Button_Body->setMat4("projection", projection);
    glBindVertexArray(this->VAO_BACK);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}    

void BUTTON::init_text()
{
    float x = 0.0f;
    float y = 0.0f;
    float z = -0.09f;

    float scale = 0.5f;

    glm::vec3 color = glm::vec3(0.0f);   

    unsigned int textModelLoc = glGetUniformLocation(Button_Text->ID, "model");
    unsigned int textViewLoc  = glGetUniformLocation(Button_Text->ID, "view");
   
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(textModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(textViewLoc, 1, GL_FALSE, &view[0][0]);

    glGenVertexArrays(1, &this->VAOTxt);
    glGenBuffers(1, &VBOTxt);
    glBindVertexArray(this->VAOTxt);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTxt);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
    
    // Use Text Shader
    Button_Text->use();
    Button_Text->setMat4("model", model);
    Button_Text->setMat4("view", view);
    Button_Text->setMat4("projection", projection);
 
    Button_Text->setVec3("textColor", color);

    unsigned int char_count = 0;
    // iterate through all characters
    int n = 0;
    float str_height = 0.0f;
    float str_width = 0.0f;

    std::string::const_iterator c;

    //Find Bounding Box of String
    for (c = label.begin(); c != label.end(); c++) 
    {
        Character ch = (*Characters)[*c];

        str_width += ch.Size.x;
        str_height = std::max(str_height, (float)ch.Size.y);
    }
    // Placement appears off center Adding two extra Characters
    str_width += (*Characters)['D'].Size.x * 2;

    for (c = label.begin(); c != label.end(); c++) 
    {
        Character ch = (*Characters)[*c];
        
        float xpos = x + (ch.Bearing.x - (str_width / 2.0f)) * scale / (SCREEN_WIDTH / 2.0f);
        float ypos = y - ((ch.Size.y - ch.Bearing.y) * scale + (str_height / 2.0f)) / (SCREEN_HEIGHT / 2.0f);
    
        float w = ((ch.Size.x * scale)) / (SCREEN_WIDTH / 2.0f);
        float h = ((ch.Size.y * scale)) / (SCREEN_HEIGHT / 2.0f);
        
        float vertices[6][5] = {
        //  x         y         z        u    v
            { xpos,     ypos + h, z + press,     0.0, 0.0 },
            { xpos + w, ypos,     z + press,     1.0, 1.0 },
            { xpos,     ypos,     z + press,     0.0, 1.0 },

            { xpos,     ypos + h, z + press,     0.0, 0.0 },
            { xpos + w, ypos + h, z + press,     1.0, 0.0 },
            { xpos + w, ypos,     z + press,     1.0, 1.0 }
        };
        
        // render glyph texture over quad
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBOTxt);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindVertexArray(VAOTxt);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale/(SCREEN_WIDTH / 2.0f);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void BUTTON::draw_text()
{
    float x = 0.0f;
    float y = 0.0f;
    float z = -0.09f;

    float scale = 1.0f;

    glm::vec3 color = glm::vec3(0.0f);   

    unsigned int textModelLoc = glGetUniformLocation(Button_Text->ID, "model");
    unsigned int textViewLoc  = glGetUniformLocation(Button_Text->ID, "view");
   
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(textModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(textViewLoc, 1, GL_FALSE, &view[0][0]);

        // Use Text Shader
    Button_Text->use();
    Button_Text->setMat4("model", model);
    Button_Text->setMat4("view", view);
    Button_Text->setMat4("projection", projection);
 
    Button_Text->setVec3("textColor", color);

    unsigned int char_count = 0;
    // iterate through all characters
    int n = 0;
    float str_height = 0.0f;
    float str_width = 0.0f;

    std::string::const_iterator c;

    //Find Bounding Box of String
    for (c = label.begin(); c != label.end(); c++) 
    {
        Character ch = (*Characters)[*c];

        str_width += ch.Size.x;
        str_height = std::max(str_height, (float)ch.Size.y);
    }
    // Placement appears off center Adding two extra Characters
    str_width += (*Characters)['D'].Size.x * 2;

    for (c = label.begin(); c != label.end(); c++) 
    {
        Character ch = (*Characters)[*c];
        
        float xpos = x + (ch.Bearing.x - (str_width / 2.0f))* scale / (SCREEN_WIDTH / 2.0f);
        float ypos = y - ((ch.Size.y - ch.Bearing.y) * scale + (str_height / 2.0f)) / (SCREEN_HEIGHT / 2.0f);

        float w = (ch.Size.x * scale) / (SCREEN_WIDTH / 2.0f);
        float h = (ch.Size.y * scale) / (SCREEN_HEIGHT / 2.0f);

        float vertices[6][5] = {
        //  x         y         z        u    v
            { xpos,     ypos + h, z + press,     0.0, 0.0 },
            { xpos + w, ypos,     z + press,     1.0, 1.0 },
            { xpos,     ypos,     z + press,     0.0, 1.0 },

            { xpos,     ypos + h, z + press,     0.0, 0.0 },
            { xpos + w, ypos + h, z + press,     1.0, 0.0 },
            { xpos + w, ypos,     z + press,     1.0, 1.0 }
        };

        // render glyph texture over quad
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBOTxt);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindVertexArray(VAOTxt);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale/(SCREEN_WIDTH / 2.0f);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void BUTTON::init_body(){
    // This renders the Boddy of the button Sides Facets and Face
    float vectors[486] = {
        // Coord                 //color                 //texture

        -0.75f,  0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f, // SIDES
        -0.75f,  0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
        -0.75f, -0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
        -0.75f, -0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
        -0.75f, -0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
        -0.75f,  0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,

         0.75f,  0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
         0.75f,  0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
         0.75f, -0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
         0.75f, -0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
         0.75f, -0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
         0.75f,  0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,

        -0.75f, -0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
         0.75f, -0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
         0.75f, -0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
         0.75f, -0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
        -0.75f, -0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
        -0.75f, -0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,

        -0.75f,  0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
         0.75f,  0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,
         0.75f,  0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
         0.75f,  0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
        -0.75f,  0.25f,  0.0f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.4f,  0.0f, 0.0f,
        -0.75f,  0.25f, -0.1f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.9f,  0.0f, 0.0f,

        -0.65f,  0.15f,  0.05f + press,  Button_Color.x + r + ring , Button_Color.y + g + ring, Button_Color.z + b + ring, 0.35f,  0.0f, 0.0f, // SLANT
        -0.75f,  0.25f,   0.0f + press,  Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.4f,   0.0f, 0.0f,
        -0.75f, -0.25f,   0.0f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.4f,   0.0f, 0.0f,
        -0.75f, -0.25f,   0.0f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.4f,   0.0f, 0.0f,
        -0.65f, -0.15f,  0.05f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.35f,  0.0f, 0.0f,
        -0.65f,  0.15f,  0.05f + press,  Button_Color.x + r  + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.35f,  0.0f, 0.0f,

         0.65f,  0.15f,  0.05f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.35f,  0.0f, 0.0f,
         0.75f,  0.25f,   0.0f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.4f,   0.0f, 0.0f,
         0.75f, -0.25f,   0.0f + press,  Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.4f,   0.0f, 0.0f,
         0.75f, -0.25f,   0.0f + press,  Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.4f,   0.0f, 0.0f,
         0.65f, -0.15f,  0.05f + press,  Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.35f,  0.0f, 0.0f,
         0.65f,  0.15f,  0.05f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.35f,  0.0f, 0.0f,

        -0.75f, -0.25f,   0.0f + press,  Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.4f,   0.0f, 0.0f,
         0.75f, -0.25f,   0.0f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.4f,   0.0f, 0.0f,
         0.65f, -0.15f,  0.05f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.35f,  0.0f, 0.0f,
         0.65f, -0.15f,  0.05f + press,  Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.35f,  0.0f, 0.0f,
        -0.65f, -0.15f,  0.05f + press,  Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.35f,  0.0f, 0.0f,
        -0.75f, -0.25f,   0.0f + press,  Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.4f,   0.0f, 0.0f,

        -0.75f,  0.25f,  0.0f + press,   Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.4f,   0.0f, 0.0f,
         0.75f,  0.25f,  0.0f + press,   Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.4f,   0.0f, 0.0f,
         0.65f,  0.15f, 0.05f + press,   Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.35f,  0.0f, 0.0f,
         0.65f,  0.15f, 0.05f + press,   Button_Color.x + r + ring, Button_Color.y + g + ring, Button_Color.z + b + ring, 0.35f,  0.0f, 0.0f,
        -0.65f,  0.15f, 0.05f + press,   Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.35f,  0.0f, 0.0f,
        -0.75f,  0.25f,  0.0f + press,   Button_Color.x + ring, Button_Color.y + ring, Button_Color.z + ring, 0.4f,   0.0f, 0.0f,

        -0.65f, -0.15f,  0.05f + press,  Button_Color.x + r, Button_Color.y + g, Button_Color.z + b, 0.3f,  0.0f, 0.0f, // Front
         0.65f, -0.15f,  0.05f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.3f,  0.0f, 0.0f,
         0.65f,  0.15f,  0.05f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.3f,  0.0f, 0.0f,
         0.65f,  0.15f,  0.05f + press,  Button_Color.x, Button_Color.y, Button_Color.z, 0.3f,  0.0f, 0.0f,
        -0.65f,  0.15f,  0.05f + press,  Button_Color.x + r, Button_Color.y + g, Button_Color.z + b, 0.3f,  0.0f, 0.0f,
        -0.65f, -0.15f,  0.05f + press,  Button_Color.x + r, Button_Color.y + g, Button_Color.z + b, 0.3f,  0.0f, 0.0f        
    };

    // Use the body shader
    unsigned int VBO;
    glGenVertexArrays(1, &this->VAO_Body);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(this->VAO_Body);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void BUTTON::draw_body()
{
    Button_Body->use();
    unsigned int modelLoc = glGetUniformLocation(Button_Body->ID, "model");
    unsigned int viewLoc  = glGetUniformLocation(Button_Body->ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    Button_Body->setMat4("projection", projection); 

    // render
    // ------
    glBindVertexArray(this->VAO_Body);
    glDrawArrays(GL_TRIANGLES, 0, 54);  
}

void BUTTON::handleEvent( SDL_Event* e, int &x, int &y )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		SDL_GetMouseState( &x, &y );
        if(press < 0.0f){
            press = 0.0f;
            init_button(); 
            gRenderQuad = true;
        }

        SDL_LockMutex(counterLock);
        if(timeRemain < 0.01f && playing > 0.0f){
            playing = 0.0f;
            init_button();
            gRenderQuad = true;
        }
        SDL_UnlockMutex(counterLock);

		//Check if mouse is in button
		bool inside = true & (!esc_button) & (!button_window);

		//Mouse is left of the button
		if( x < T_Left.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > B_Right.x )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < T_Left.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > B_Right.y )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			ring = 0.0f;
            if(prev_visit){
                gRenderQuad = true;
                prev_visit = false;
                init_button();
            }
		}
		//Mouse is inside button
		else
		{
            // light facets
            ring = 0.25f;
            // Avoid excessive re-rendering
            if(!prev_visit){
                gRenderQuad = true;
                init_button();
            }
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				//mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			    
				case SDL_MOUSEBUTTONDOWN:
	
                if(e->button.button == SDL_BUTTON_LEFT){
                    SDL_LockMutex(counterLock);
                    if(sample != NULL && timeRemain < 0.01f){
                        countID myID;
                        myID.ID = &ID;
                        myID.timeRemain = &timeRemain;
                        ButtonID->push_back(myID);
                        playing = 0.3f;
                        timeRemain = sampleDuration;
                        init_button();
                        Mix_PlayChannel( -1, sample, 0 );
                    }
                    SDL_UnlockMutex(counterLock);

                    press = -0.05f;
                    init_button();
                    gRenderQuad = true;
                }

                if(e->button.button == SDL_BUTTON_RIGHT){
                    button_win_ID = ID;
                    float xt = (((x-8)*2/((float)SCREEN_WIDTH)) - 1)*SXTN_X_NINE*TAN_35DEG -(-0.5)*SCALE_OPT_WIN;
                    float yt = -(((y-8)*2/((float)SCREEN_HEIGHT)) - 1)*TAN_35DEG - 1*SCALE_OPT_WIN;
                    //printf("xt: %f yt: %f\n", xt, yt);

                    // Prevent opt_window from going through bottom of render window.
                    if(y > 208){
                        yt = -0.20435681720035f;    
                    }
                    
                    Button_Window->model = glm::mat4(1.0);
                    Button_Window->model = glm::translate(Button_Window->model,glm::vec3(xt, yt, -1.0f));
                    Button_Window->model = glm::scale(Button_Window->model, glm::vec3(SCALE_OPT_WIN));
                    Button_Window->calc_bound_box();
                    Button_Window->fields[0].label = label;
                    Button_Window->set_field_values();
                    button_window = true;
                    gRenderQuad = true;
                }          

				break;
				
				case SDL_MOUSEBUTTONUP:
				//mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
            prev_visit = true;
		}
	}
}

