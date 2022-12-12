#include "opt_window.h"
//#include "opt_field.h"


OPT_WINDOW::OPT_WINDOW()
{

}

OPT_WINDOW::OPT_WINDOW(Shader *win_shader, Shader *text, std::map<char, Character> *Chars, WINDOW_TYPE TYPE)
{
    W_TYPE = TYPE;
    shader = win_shader;
    text_shader = text;
    Characters = Chars;
    projection = glm::mat4(1.0f);//glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
    model = glm::mat4(1.0f);
    init_window_body();
    
}

void OPT_WINDOW::init_window_body()
{
        // Defines The Window Size
    float z = 0.0f;
    float transparency = 0.75f;
    float win_body[210] = {
        -0.48f, -0.98f, z,  0.1f, 0.1f, 0.1f, transparency, // Main Window Body
         0.48f, -0.98f, z,  0.1f, 0.1f, 0.1f, transparency,
         0.48f,  0.98f, z,  0.1f, 0.1f, 0.1f, transparency,
         0.48f,  0.98f, z,  0.1f, 0.1f, 0.1f, transparency,
        -0.48f,  0.98f, z,  0.1f, 0.1f, 0.1f, transparency,
        -0.48f, -0.98f, z,  0.1f, 0.1f, 0.1f, transparency,

        -0.5f, -1.0f,   z, 0.5f, 0.5f, 0.5f, 1.0f, // Left Side
        -0.48f,-0.98f,  z, 0.5f, 0.5f, 0.5f, 1.0f,
        -0.48f, 0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.48f, 0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
        
        -0.48f, 0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f, // Top Side
         0.48f, 0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
        -0.48f, 0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f,
        
         0.48f,-0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f, // Right Side
         0.5f, -1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.5f,  1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.48f, 0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.48f,-0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f, // Bottom Side
         0.5f, -1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.48f,-0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.48f,-0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f,
        -0.48f,-0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f
    };
    
    unsigned int VBO;
    // Use my button Shaders
    shader->use();
    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    //unsigned int modelLoc = glGetUniformLocation(Button_Body->ID, "model");
    //unsigned int viewLoc  = glGetUniformLocation(Button_Body->ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    shader->setMat4("model", model);
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader->setMat4("projection", projection);
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(win_body), win_body, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    //glEnableVertexAttribArray(2);    
    // render
    // ------    
    /*
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glBindVertexArray(this->VAO_BACK);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);*/   
    //glm::vec4 test = projection * model * glm::vec4(-0.5f, 1.0f, 1.0, 1.0f);
    //printf("test.x: %f test.y: %f test.z: %f test.w: %f\n", test.x, test.y, test.z, test.w); 
}

void OPT_WINDOW::init_menu_window_body()
{
    // Defines The Window Size
    float z = 0.0f;
    float transparency = 0.75f;
    float win_body[210] = {
        -0.73f, -0.98f, z,  0.1f, 0.1f, 0.1f, transparency, // Main Window Body
         0.73f, -0.98f, z,  0.1f, 0.1f, 0.1f, transparency,
         0.73f,  0.98f, z,  0.1f, 0.1f, 0.1f, transparency,
         0.73f,  0.98f, z,  0.1f, 0.1f, 0.1f, transparency,
        -0.73f,  0.98f, z,  0.1f, 0.1f, 0.1f, transparency,
        -0.73f, -0.98f, z,  0.1f, 0.1f, 0.1f, transparency,

        -0.75f, -1.0f,   z, 0.5f, 0.5f, 0.5f, 1.0f, // Left Side
        -0.73f,-0.98f,  z, 0.5f, 0.5f, 0.5f, 1.0f,
        -0.73f, 0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.73f, 0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.75f,  1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.75f, -1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
        
        -0.73f, 0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f, // Top Side
         0.73f, 0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,
         0.75f,  1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f,
         0.75f,  1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.75f,  1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
        -0.73f, 0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f,
        
         0.73f,-0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f, // Right Side
         0.75f, -1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f,
         0.75f,  1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.75f,  1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.73f, 0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.73f,-0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,

        -0.75f, -1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f, // Bottom Side
         0.75f, -1.0f,  z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.73f,-0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f,
         0.73f,-0.98f, z,  0.5f, 0.5f, 0.5f, 1.0f,
        -0.73f,-0.98f, z,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.75f, -1.0f,  z,  1.0f, 1.0f, 1.0f, 1.0f
    };
    
    unsigned int VBO;
    // Use my button Shaders
    shader->use();
    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    //unsigned int modelLoc = glGetUniformLocation(Button_Body->ID, "model");
    //unsigned int viewLoc  = glGetUniformLocation(Button_Body->ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    shader->setMat4("model", model);
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader->setMat4("projection", projection);
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(win_body), win_body, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    //glEnableVertexAttribArray(2);    
    // render
    // ------    
    /*
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glBindVertexArray(this->VAO_BACK);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);*/   
    //glm::vec4 test = projection * model * glm::vec4(-0.5f, 1.0f, 1.0, 1.0f);
    //printf("test.x: %f test.y: %f test.z: %f test.w: %f\n", test.x, test.y, test.z, test.w); 
}

void OPT_WINDOW::draw_window_body()
{
    // Use my button Shaders
    shader->use();
    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    //unsigned int viewLoc  = glGetUniformLocation(Button_Body->ID, "view");
    // pass them to the shaders (3 different ways)
    /* I really really want to fix this so I can pass my shader parameters by pointer */
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader->setMat4("model", model);
    shader->setMat4("projection", projection);
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 30);
}

void OPT_WINDOW::draw_window()
{
    draw_window_body();
    for(int i = 0;i < fields.size();i++){
        fields[i].draw_field();
        //draw_window_body();
        //printf("%s\n", fields[i].label.c_str());
        fields[i].draw_text(0.0f, 0.98f - 0.05- fields[i].y_off*fields[i].FIELD_ID, 0.0f, glm::vec3(0.5, 0.5, 0.5));  
    }
    //printf("fields.size(): %d\n", fields.size());
}

void OPT_WINDOW::init_fields()
{
    if(W_TYPE == ESC_WIN){
        for(int i = 0; i < 4; i++){
            OPT_FIELD field(shader, text_shader, 0.0, 0.0, i, Characters, ESC_WIN);
            field.model = model;
            field.projection = projection;
            field.init_field();
            fields.push_back(field);
        }
        fields[0].label = "NEW";
        fields[0].TYPE = NEW;
        fields[1].label = "SAVE";
        fields[1].TYPE = SAVE;
        fields[2].label = "LOAD";
        fields[2].TYPE = LOAD;
        fields[3].label = "EXIT";
        fields[3].TYPE = EXIT;
    } else if(W_TYPE == BUTTON_WIN){
        for(int i = 0; i < 8; i++){
            OPT_FIELD field(shader, text_shader, 0.0f, 0.0f, i, Characters, BUTTON_WIN);
            field.model = model;
            field.projection = projection;
            switch(i){
                case 0:
                    field.label = "Field" + std::to_string(i);
                    field.TYPE = LABEL;
                    break;
                case 1:
                    field.label = "Sample Select";
                    field.TYPE = SAMPLE;
                    break;
                case 2:
                    field.label = "Color";
                    field.TYPE = COLOR;
                    break;
                case 3:
                    field.label = "Remove";
                    field.TYPE = REMOVE;
                    break;
                default:
                   field.label = "Field" + std::to_string(i); 
                   field.TYPE = NONE;
                   break;
            }
            field.init_field();
            fields.push_back(field);
        }
    } else {
        printf("you can't handle muh menu\n");
    }
}

void OPT_WINDOW::set_field_values()
{
    for(int i = 0; i < fields.size(); i++){
        fields[i].model = model;
        fields[i].projection = projection;
        fields[i].init_field();
    }
}

void OPT_WINDOW::calc_bound_box()
{
    glm::vec4 TL = projection * model * TL_Corner; 
    glm::vec4 BR = projection * model * BR_Corner;
    S_COORD_TL.x = (TL.x + 1.0f) * (SCREEN_WIDTH / 2.0f);
    S_COORD_TL.y = (-TL.y + 1.0f) * (SCREEN_HEIGHT / 2.0f);
    S_COORD_BR.x = (BR.x + 1.0f) * (SCREEN_WIDTH / 2.0f);
    S_COORD_BR.y = (-BR.y + 1.0f) * (SCREEN_HEIGHT / 2.0f);
}

/*void OPT_WINDOW::handleEvent( SDL_Event* e, int &x, int &y )
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

        SDL_LockMutex(counterLock);
        if(timeRemain < 0.01f && playing > 0.0f){
            playing = 0.0f;
            init_button();
            gRenderQuad = true;
        }
        SDL_UnlockMutex(counterLock);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < field.x )
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
				break;
				
				case SDL_MOUSEBUTTONUP:
				//mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
            prev_visit = true;
		}
	}
}*/