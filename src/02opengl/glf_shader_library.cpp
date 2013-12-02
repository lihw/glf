// -------------------------------------------------------------- 
// glf_shader_library.cpp
// The OpenGL error handling.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_shader_library.h"

GLF_NAMESPACE_BEGIN

namespace ShaderLibrary
{

char* VS_SOURCE[] =
{
    // -------------------------------------------------------------- 
    // None
    // -------------------------------------------------------------- 
    "",
    // -------------------------------------------------------------- 
    // Color
    // -------------------------------------------------------------- 
    "#version 410 core \n"
    "\n"
    "layout (location = 0) in vec3 Position;\n"
    "\n"
    "uniform mat4 MVP;\n"
    "\n"
    "out gl_PerVertex \n"
    "{\n"
    "    vec4 gl_Position;\n"
    "};\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(Position, 1); \n"
    "}\n",
};

char* FS_SOURCE[] =
{
    // -------------------------------------------------------------- 
    // None
    // -------------------------------------------------------------- 
    "",
    // -------------------------------------------------------------- 
    // Color
    // -------------------------------------------------------------- 
    "#version 410 core \n"
    "uniform vec4 Color;\n"
    "\n"
    "layout (location = 0, index = 0) out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragColor = Color; \n"
    "}\n",
};

char* GS_SOURCE[SHADER_NUMBER];
char* TES_SOURCE[SHADER_NUMBER];
char* TCS_SOURCE[SHADER_NUMBER];


}; // namespace ShaderLibrary



GLF_NAMESPACE_END


