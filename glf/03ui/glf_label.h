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

class Label : public Widget
{
public:
    Label(const char* name, GLuint x, GLuint y, const GLubyte* color);
    virtual ~Label();

    virtual void render();
};

GLF_NAMESPACE_END


#endif // !GLF_LABEL_H
