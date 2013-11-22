// -------------------------------------------------------------- 
// glf_bitmap_font.h
// A fixed bitmap for displaying the rendering information, like FPS.
// The font only contains ASCII 32 (space) to ASCII 126 (tide ~).
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_BITMAP_FONT_H
#define GLF_BITMAP_FONT_H

#include "../glf_common.h"

#include "../01api/glf_texture.h"
#include "../02utility/glf_rect.h"

GLF_NAMESPACE_BEGIN

class Texture;
class Rect;

#define BITMAP_FONT_FIRST_CHARACTER 32
#define BITMAP_FONT_LAST_CHARACTER 126
#define BITMAP_FONT_NUMBER (BITMAP_FONT_LAST_CHARACTER - BITMAP_FONT_FIRST_CHARACTER + 1)

class BitmapFont
{
public:
    // Constructor.
    BitmapFont();

    // Destructor. Release all resources.
    ~BitmapFont();

    // Draw a text onto the screen. The characters not supported
    // are replaced with "?".
    // \param text the text message.
    // \param x the x coordinate of left bottom corner of the text to draw.
    // \param y ditto.
    void drawText(const char* text, GLuint x, GLuint y, const GLfloat* color);

    // Get the font width and height.
    GLuint getWidth(char c) const
    GLuint getHeight(char c) const

private:
    // Create the font texture, shader and geometry.
    void create(GLubyte* fontData);

    // Draw the character at the position.
    void drawCharacter(char character, GLuint x, GLuint y);

private:
    Texture*     _textures[BITMAP_FONT_NUMBER];
    Rect*        _rect;
    glm::mat4    _projection;
};

GLF_NAMESPACE_END

#endif // !GLF_BITMAP_FONT_H
