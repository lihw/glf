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


Label::Label(const char* name, GLint x, GLint y, const GLubyte* color)
    : Widget(name)
    , _font()
{
    setPosition(x, y);
    setFgColor(color);
}

Label::~Label()
{
}

void Label::render()
{

}
    
GLF_NAMESPACE_END
