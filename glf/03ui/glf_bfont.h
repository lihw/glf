// -------------------------------------------------------------- 
// glf_bfont.h
// A fixed bitmap for displaying the rendering information, like FPS.
// The font only contains ASCII 32 (space) to ASCII 126 (tide ~).
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_BFONT_H
#define GLF_BFONT_H

#include "../glf_common.h"

#include "../01api/glf_texture.h"
#include "../02utility/glf_rect.h"

GLF_NAMESPACE_BEGIN

class Texture;
class Rect;

enum BitmapFontEnum
{
    BITMAP_FONT_8X13,
    BITMAP_FONT_9X15,
};

class BFont
{
public:
    // Constructor.
    BFont(BitmapFontEnum font);

    // Destructor. Release all resources.
    ~BFont();

    // Draw a text onto the screen. The characters not in
    // ASCII 32 to 126 are replaced with "?".
    // \param text the text message.
    // \param x the x coordinate of left bottom corner of the text to draw.
    // \param y ditto.
    void drawText(const char* text, GLuint x, GLuint y, const GLfloat* color);

    // Get the font width.
    GLF_INLINE GLuint getWidth() const
    { return _fontSize[0]; }

    // Get the font height
    GLF_INLINE GLuint getHeight() const
    { return _fontSize[1]; }

private:
    // Create the font texture, shader and geometry.
    void create(GLubyte* fontData);

    // Draw the character at the position.
    void drawCharacter(char character, GLuint x, GLuint y);

    // Expand a bitmap to a gray image.
    // \param bitmap the bitmap
    // \param image the expanded result.
    void expandBitmap(const GLubyte* bitmap, GLubyte* image) const;

private:
    Texture*     _texture;
    Rect*        _rect;
    glm::mat4    _projection;
    GLuint       _fontSize[2];
};

GLF_NAMESPACE_END

#endif // !GLF_BFONT_H
