// -------------------------------------------------------------- 
// glf_image_tga.cpp
// Read and write TGA image.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_image_tga_private.h"

#include "../glf_assert.h"

bool GLF_APIENTRY glfReadTGA(FILE* fp, GLuint& width, GLuint& height, GLuint& nchannels, GLubyte*& data)
{
    GLF_ASSERT_NOT_IMPLEMENTED();
    return false;
}

bool GLF_APIENTRY glfWriteTGA(FILE* fp, GLuint width, GLuint height, GLuint nchannels, const GLubyte* data)
{
    GLF_ASSERT_NOT_IMPLEMENTED();
    return false;
}
