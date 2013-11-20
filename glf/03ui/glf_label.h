// -------------------------------------------------------------- 
// glf_label.h
// A static label
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#ifndef GLF_LABEL_H
#define GLF_LABEL_H

#include "../glf_common.h"

#include "../03ui/glf_bfont.h"

GLF_NAMESPACE_BEGIN

class Label
{
public:
    Label(const char* labelText, GLuint x, GLuint y, const GLfloat* color);
    ~Label();

    void setText(const char* text);
    void setColor(const GLfloat* color);
    void setPosition(GLuint x, GLuint y);

    void render();

private:
    char*   _text;
    GLuint  _x;
    GLuint  _y;
    BFont   _font;
    GLfloat _color[4];
};

GLF_NAMESPACE_END


#endif // !GLF_LABEL_H
