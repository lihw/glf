// -------------------------------------------------------------- 
// glf_image_tga_private.h
// Read and write TGA image.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_IMAGE_TGA_PRIVATE_H
#define GLF_IMAGE_TGA_PRIVATE_H

#include "../../glf_common.h"

#include <cstdio>

GLF_EXTERN bool GLF_APIENTRY glfReadTGA(FILE* fp, GLuint& width, GLuint& height, GLuint& nchannels, GLubyte*& data);

GLF_EXTERN bool GLF_APIENTRY glfWriteTGA(FILE* fp, GLuint width, GLuint height, GLuint nchannels, const GLubyte* data); 


#endif // !GLF_IMAGE_TGA_PRIVATE_H
