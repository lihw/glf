// -------------------------------------------------------------- 
// glf_image_png_private.h
// Read and write PNG image.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_IMAGE_PNG_PRIVATE_H
#define GLF_IMAGE_PNG_PRIVATE_H

#include "../../glf_common.h"

#include <cstdio>

GLF_EXTERN bool GLF_APIENTRY glfReadPNG(FILE* fp, GLuint& width, GLuint& height, GLuint& nchannels, GLubyte*& data);

GLF_EXTERN bool GLF_APIENTRY glfWritePNG(FILE* fp, GLuint width, GLuint height, GLuint nchannels, 
    const GLubyte* data); 


#endif // !GLF_IMAGE_PNG_PRIVATE_H
