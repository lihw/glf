// -------------------------------------------------------------- 
// glf_glerror.cpp
// The OpenGL error handling.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_glerror.h"

#include "../01system/glf_log.h"

GLF_NAMESPACE_BEGIN

bool checkGLError(const char* tag)
{
    GLenum err = glGetError();
    if (err == GL_NO_ERROR)
    {
        return true;
    }

    int errmsg = 0;

    static const char* ERRMSG[] =
    {
        "GL_INVALID_ENUM",
        "GL_INVALID_FRAMEBUFFER_OPERATION",
        "GL_INVALID_VALUE",
        "GL_INVALID_OPERATION",
        "GL_OUT_OF_MEMORY",
    };

    switch (err)
    {
        case GL_INVALID_ENUM: errmsg = 0;  break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errmsg = 1; break;
        case GL_INVALID_VALUE: errmsg = 2;  break;
        case GL_INVALID_OPERATION: errmsg = 3;  break;
        case GL_OUT_OF_MEMORY: errmsg = 4;  break;
    }

    GLF_LOGINFO("GL Errror: %s at %s!\n", ERRMSG[errmsg], tag);

    return false;
}

GLF_NAMESPACE_END
