#include "opt_field.h"

OPT_FIELD::OPT_FIELD()
{

}

OPT_FIELD::OPT_FIELD(Shader *field_shader, Shader *text, float x, float y, int ID,std::map<char, Character> *Chars, WINDOW_TYPE W_TYPE)
{
    shader = field_shader;
    text_shader = text;
    Characters = Chars;
    FIELD_ID = ID;
    init_text(0.0, .98 - 0.10875, 0, glm::vec3(1.0));
}

void OPT_FIELD::init_field()
{
    // Defines the Back of the Button Must be drawn first
    //printf("Enter_INIT\n");
    float z = 0.0f;
    float transparency = 0.75f;
    float bx_upr = (float)FIELD_ID*y_off;
    float bx_lwr = y_off*(FIELD_ID + 1) - 0.02f;
    //printf("%s: ID: %d bx_upr: %f bx_lwr: %f\n", label.c_str(), FIELD_ID, bx_upr, bx_lwr);
    float field_body[84] = {
        -0.46f,  0.97f - bx_lwr, z,  0.1f + MOE, 0.1f + MOE, 0.1f + MOE, transparency, // Main Window Body
         0.46f,  0.97f - bx_lwr, z,  0.1f + MOE, 0.1f + MOE, 0.1f + MOE, transparency,
         0.46f,  0.97f - bx_upr, z,  0.1f + MOE, 0.1f + MOE, 0.1f + MOE, transparency,
         0.46f,  0.97f - bx_upr, z,  0.1f + MOE, 0.1f + MOE, 0.1f + MOE, transparency,
        -0.46f,  0.97f - bx_upr, z,  0.1f + MOE, 0.1f + MOE, 0.1f + MOE, transparency,
        -0.46f,  0.97f - bx_lwr, z,  0.1f + MOE, 0.1f + MOE, 0.1f + MOE, transparency,

        -0.46f, 0.97f - y_off * (FIELD_ID + 1),  z,  1.0f, 1.0f, 1.0f, 1.0f, // Bottom Side
         0.46f, 0.97f - y_off * (FIELD_ID + 1),  z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.46f, 0.97f - bx_lwr, z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.46f, 0.97f - bx_lwr, z,  0.5f, 0.5f, 0.5f, 1.0f,
        -0.46f, 0.97f - bx_lwr, z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.46f, 0.97f - y_off * (FIELD_ID + 1),  z,  1.0f, 1.0f, 1.0f, 1.0f
    };
    TL = projection * model * glm::vec4(field_body[0], field_body[15], z, 1.0f);
    BR = projection * model * glm::vec4(field_body[7], field_body[43], z, 1.0f);
    bounding_box();

    // Use my button Shaders
    shader->use();
    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");

    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    shader->setMat4("model", model);

    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader->setMat4("projection", projection);
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(field_body), field_body, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void OPT_FIELD::draw_field()
{
    // Use my button Shaders
    shader->use();
    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader->setMat4("model", model);
    shader->setMat4("projection", projection);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);  
}

void OPT_FIELD::init_text(float x, float y, float z, glm::vec3 color)
{

    float scale = 0.5f;
    color_text = color;

    // Use Text Shader
    text_shader->use();

    unsigned int textModelLoc = glGetUniformLocation(text_shader->ID, "model");

    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(textModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    //unsigned int VBO;
    glGenVertexArrays(1, &this->VAOTxt);
    glGenBuffers(1, &VBOTxt);
    glBindVertexArray(this->VAOTxt);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTxt);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
    //glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    // Use Text Shader
    text_shader->use();
    text_shader->setMat4("model", model);
    //text_shader->setMat4("view", view);
    text_shader->setMat4("projection", projection);
 
    text_shader->setVec3("textColor", color_text);
    
    unsigned int char_count = 0;
    // iterate through all characters
    //int n = 0;
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
    str_width += (*Characters)['D'].Size.x;

    //unsigned int VBO;
    for (c = label.begin(); c != label.end(); c++) 
    {
        Character ch = (*Characters)[*c];
        
        float xpos = x + (ch.Bearing.x - (str_width / 2.0f))* scale / (SCREEN_WIDTH / 2.0f);
        float ypos = y - ((ch.Size.y - ch.Bearing.y) * scale + (str_height / 2.0f)) / (SCREEN_HEIGHT / 2.0f);
        //float ypos = y - (ch.Size.y - ch.Bearing.y + (str_height / 2.0f) * scale) / (SCREEN_HEIGHT / 2.0f);

        float w = ((ch.Size.x * scale)) / (SCREEN_WIDTH / 2.0f);
        float h = ((ch.Size.y * scale)) / (SCREEN_HEIGHT / 2.0f);

        float vertices[6][5] = {
        //  x         y         z        u    v
            { xpos,     ypos + h, z,     0.0, 0.0 },
            { xpos + w, ypos,     z,     1.0, 1.0 },
            { xpos,     ypos,     z,     0.0, 1.0 },

            { xpos,     ypos + h, z,     0.0, 0.0 },
            { xpos + w, ypos + h, z,     1.0, 0.0 },
            { xpos + w, ypos,     z,     1.0, 1.0 }
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
        x += (ch.Advance >> 6) * scale/(SCREEN_WIDTH / 2.0f); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OPT_FIELD::draw_text(float x, float y, float z, glm::vec3 color)
{
    float scale = 0.5f;

    unsigned int textModelLoc = glGetUniformLocation(text_shader->ID, "model");
   
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(textModelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Use Text Shader
    text_shader->use();
    text_shader->setMat4("model", model);
    text_shader->setMat4("projection", projection);
 
    text_shader->setVec3("textColor", color_text);
    //unsigned int VBO;
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
    str_width += (*Characters)['D'].Size.x;

    //printf("width: %f, height: %f\n", str_width, str_height);
    //unsigned int VBO;
    for (c = label.begin(); c != label.end(); c++) 
    {
        Character ch = (*Characters)[*c];
        
        float xpos = x + (ch.Bearing.x - (str_width / 2.0f))* scale / (SCREEN_WIDTH / 2.0f);
        float ypos = y - ((ch.Size.y - ch.Bearing.y) * scale + (str_height / 2.0f)) / (SCREEN_HEIGHT / 2.0f);

        float w = ((ch.Size.x * scale)) / (SCREEN_WIDTH / 2.0f);
        float h = ((ch.Size.y * scale)) / (SCREEN_HEIGHT / 2.0f);

        float vertices[6][5] = {
        //  x         y         z        u    v
            { xpos,     ypos + h, z,     0.0, 0.0 },
            { xpos + w, ypos,     z,     1.0, 1.0 },
            { xpos,     ypos,     z,     0.0, 1.0 },

            { xpos,     ypos + h, z,     0.0, 0.0 },
            { xpos + w, ypos + h, z,     1.0, 0.0 },
            { xpos + w, ypos,     z,     1.0, 1.0 }
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
        x += ((ch.Advance >> 6) * scale)/(SCREEN_WIDTH / 2.0f); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OPT_FIELD::bounding_box()
{
    TL_CORNER.x = (TL.x + 1.0f) * (SCREEN_WIDTH / 2.0f);
    TL_CORNER.y = (-TL.y + 1.0f) * (SCREEN_HEIGHT / 2.0f);
    BR_CORNER.x = (BR.x + 1.0f) * (SCREEN_WIDTH / 2.0f);
    BR_CORNER.y = (-BR.y + 1.0f) * (SCREEN_HEIGHT / 2.0f);
    //printf("TL_C.x: %d TL_C.y: %d BR_C.x: %d BR_C.y: %d\n", TL_CORNER.x, TL_CORNER.y, BR_CORNER.x, BR_CORNER.y);
}

void OPT_FIELD::handleEvent( SDL_Event* e, int &x, int &y )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		//int x, y;
		SDL_GetMouseState( &x, &y );
        //if(press < 0.0f){
        //    press = 0.0f;
        //    init_button(); 
        //    gRenderQuad = true;
        //}

        /*SDL_LockMutex(counterLock);
        if(timeRemain < 0.01f && playing > 0.0f){
            playing = 0.0f;
            init_button();
            gRenderQuad = true;
        }
        SDL_UnlockMutex(counterLock);*/

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < TL_CORNER.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > BR_CORNER.x )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < TL_CORNER.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > BR_CORNER.y )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			MOE = 0.0f;
            if(prev_visit){
                gRenderQuad = true;
                prev_visit = false;
                init_field();
            }
		}
		//Mouse is inside button
		else
		{
            // light field
            MOE = 0.25f;
            //printf("Illuminating field: %d\n", FIELD_ID);
            // Avoid excessive re-rendering
            if(!prev_visit){
                gRenderQuad = true;
                init_field();
            }
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				//mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			    
				case SDL_MOUSEBUTTONDOWN:
	
                if(e->button.button == SDL_BUTTON_LEFT){
                    /*SDL_LockMutex(counterLock);
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
                    SDL_UnlockMutex(counterLock);*/
                    if(TYPE == EXIT){
                        quit = true;
                    }
                    //press = -0.05f;
                    //init_button();
                    //gRenderQuad = true;
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