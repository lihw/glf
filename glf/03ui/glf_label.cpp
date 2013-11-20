// -------------------------------------------------------------- 
// glf_label.cpp
// A static label
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_label.h"

#include "../00system/glf_assert.h"

GLF_NAMESPACE_BEGIN


Label::Label(const char* labelText, GLuint x, GLuint y, const GLfloat* color)
    : _font(BITMAP_FONT_8X13)
{
    GLF_ASSERT(labelText != NULL);
    GLuint l = strlen(labelText);
    _text = new char [l + 1];
    strcpy(_text, labelText);

    _x = x;
    _y = y;

    GLF_ASSERT(color != NULL);
    _color[0] = color[0];
    _color[1] = color[1];
    _color[2] = color[2];
    _color[3] = color[3];
}

Label::~Label()
{
    delete [] _text;
}
    
void Label::setText(const char* text)
{
    if (_text != NULL)
    {
        delete [] _text;
    }

    GLF_ASSERT(text != NULL);
    GLuint l = strlen(text);
    _text = new char [l + 1];
    strncpy(_text, text, l);
}

void Label::setColor(const GLfloat* color)
{
    GLF_ASSERT(color != NULL);
    _color[0] = color[0];
    _color[1] = color[1];
    _color[2] = color[2];
    _color[3] = color[3];

}

void Label::setPosition(GLuint x, GLuint y)
{
    _x = x;
    _y = y;
}

void Label::render()
{
    _font.drawText(_text, _x, _y, _color);
}

GLF_NAMESPACE_END
