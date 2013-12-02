// -------------------------------------------------------------- 
// glf_shader_library.h
// The OpenGL error handling.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_SHADER_LIBRARY_H
#define GLF_SHADER_LIBRARY_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

namespace ShaderLibrary
{

enum 
{
    NONE,
    COLOR,

    SHADER_FIRST = NONE,
    SHADER_LAST = COLOR,

    SHADER_NUMBER = SHADER_LAST - SHADER_FIRST + 1,
};

GLF_EXTERN char* VS_SOURCE[SHADER_NUMBER];
GLF_EXTERN char* FS_SOURCE[SHADER_NUMBER];
GLF_EXTERN char* GS_SOURCE[SHADER_NUMBER];
GLF_EXTERN char* TES_SOURCE[SHADER_NUMBER];
GLF_EXTERN char* TCS_SOURCE[SHADER_NUMBER];

}; // namespace ShaderLibrary


GLF_NAMESPACE_END


#endif // !GLF_SHADER_LIBRARY_H
