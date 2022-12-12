#include "free_type_funcs.h"


void  Glyph_String::compute_string_bbox( FT_BBox  *abbox )
{
    FT_BBox  bbox;


    bbox.xMin = bbox.yMin =  32000;
    bbox.xMax = bbox.yMax = -32000;
    
    int n;
    for ( n = 0; n < num_glyphs; n++ )
    {
    FT_BBox  glyph_bbox;


    //FT_Glyph_Get_CBox( glyphs[n], ft_glyph_bbox_pixels,
    //                    &glyph_bbox );

    if (glyph_bbox.xMin < bbox.xMin)
        bbox.xMin = glyph_bbox.xMin;

    if (glyph_bbox.yMin < bbox.yMin)
        bbox.yMin = glyph_bbox.yMin;

    if (glyph_bbox.xMax > bbox.xMax)
        bbox.xMax = glyph_bbox.xMax;

    if (glyph_bbox.yMax > bbox.yMax)
        bbox.yMax = glyph_bbox.yMax;
    }

    if ( bbox.xMin > bbox.xMax )
    {
    bbox.xMin = 0;
    bbox.yMin = 0;
    bbox.xMax = 0;
    bbox.yMax = 0;
    }

    *abbox = bbox;
}

/*void setUpFont(FTFont *font, FTPixmapFont *infoFont)
{
    //printf(" in function before: font %u", font);
    //font = new FTBufferFont(file);
    //printf(" after: font %u\n", font);

    for(int x = 0; x < 1; ++x)
    {
        if(font->Error())
        {
            fprintf(stderr, "Failed to open font %s", "defined ttf");
            exit(1);
        }

        if(!font->FaceSize(30))
        {
            fprintf(stderr, "Failed to set size");
            exit(1);
        }

        font->Depth(3.);
        font->Outset(-.5, 1.5);

        font->CharMap(ft_encoding_unicode);
    }

    //infoFont = new FTPixmapFont(file);

    if(infoFont->Error())
    {
        fprintf(stderr, "Failed to open font %s", "defined ttf");
        exit(1);
    }

    infoFont->FaceSize(18);
/*#if 1
    strcpy(myString, "OpenGL is a powerful software interface for graphics "
           "hardware that allows graphics programmers to produce high-quality "
           "color images of 3D objects.\nabc def ghij klm nop qrs tuv wxyz "
           "ABC DEF GHIJ KLM NOP QRS TUV WXYZ 01 23 45 67 89");
#elif 0
    strcpy(myString, "OpenGL (Open Graphics Library — открытая графическая "
           "библиотека) — спецификация, определяющая независимый от языка "
           "программирования кросс-платформенный программный интерфейс "
           "для написания приложений, использующих двумерную и трехмерную "
           "компьютерную графику.");
#else
    strcpy(myString, "OpenGL™ 是行业领域中最为广泛接纳的 2D/3D 图形 API, "
           "其自诞生至今已催生了各种计算机平台及设备上的数千优秀应用程序。"
           "OpenGL™ 是独立于视窗操作系统或其它操作系统的，亦是网络透明的。"
           "在包含CAD、内容创作、能源、娱乐、游戏开发、制造业、制药业及虚拟"
           "现实等行业领域中， OpenGL™ 帮助程序员实现在 PC、工作站、超级计算"
           "机等硬件设备上的高性能、极具冲击力的高视觉表现力图形处理软件的开"
           "发。");
#endif
}*/

//FTFont *font;
//FTPixmapFont *infoFont;
//FTSimpleLayout simpleLayout;

void renderFontmetrics(FTFont *font, FTLayout **layout, char *myString)
{
    FTBBox bbox;
    float x1, y1, z1, x2, y2, z2;

    // If there is a layout, use it to compute the bbox, otherwise query as
    // a string.
    //if(layout){
    //    printf("layout case\n");
    //    bbox = layout[0]->BBox(myString);
    //}
    //else{
        bbox = font->BBox(myString);
    //}
    //printf("font %d\n", font);
    x1 = bbox.Lower().Xf(); y1 = bbox.Lower().Yf(); z1 = bbox.Lower().Zf();
    x2 = bbox.Upper().Xf(); y2 = bbox.Upper().Yf(); z2 = bbox.Upper().Zf();
    printf("x1: %f, x2: %f, y1: %f, y2: %f, z1: %f, z2: %f\n", x1, x2, y1, y2, z1, z2);

    // Draw the bounding box
    //glDisable(GL_LIGHTING);
    //glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA

    //glColor3f(0.0, 1.0, 0.0);
    // Draw the front face
    /*glBegin(GL_LINE_LOOP);
        glVertex3f(x1, y1, z1);
        glVertex3f(x1, y2, z1);
        glVertex3f(x2, y2, z1);
        glVertex3f(x2, y1, z1);
    glEnd();*/
    // Draw the back face
    /*if(current_font == FTGL_EXTRUDE && z1 != z2)
    {
        glBegin(GL_LINE_LOOP);
            glVertex3f(x1, y1, z2);
            glVertex3f(x1, y2, z2);
            glVertex3f(x2, y2, z2);
            glVertex3f(x2, y1, z2);
        glEnd();
        // Join the faces
        glBegin(GL_LINES);
            glVertex3f(x1, y1, z1);
            glVertex3f(x1, y1, z2);

            glVertex3f(x1, y2, z1);
            glVertex3f(x1, y2, z2);

            glVertex3f(x2, y2, z1);
            glVertex3f(x2, y2, z2);

            glVertex3f(x2, y1, z1);
            glVertex3f(x2, y1, z2);
        glEnd();
    }*/

    // Render layout-specific metrics
    //if(!layout)
    {
        // There is no layout. Draw the baseline, Ascender and Descender
        /*glBegin(GL_LINES);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(font->Advance(myString), 0.0, 0.0);
            glVertex3f(0.0, font->Ascender(), 0.0);
            glVertex3f(0.0, font->Descender(), 0.0);
        glEnd();*/
    }
    //else if(layout
    //         && (dynamic_cast <FTSimpleLayout *>(layout)))
    {
    //    float lineWidth = ((FTSimpleLayout *)layout)->GetLineLength();

        // The layout is a SimpleLayout.  Render guides that mark the edges
        // of the wrap region.
        /*glColor3f(0.5, 1.0, 1.0);
        glBegin(GL_LINES);
            glVertex3f(0, 10000, 0);
            glVertex3f(0, -10000, 0);
            glVertex3f(lineWidth, 10000, 0);
            glVertex3f(lineWidth, -10000, 0);
        glEnd();*/
    }

    /*// Draw the origin
    glTranslatef(-OX, -OY,0);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
        glVertex3f(0.0, 0.0, 0.0);
    glEnd();
    // Draw the axis
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
    glEnd();
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
    glEnd();
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
    glEnd();*/
}