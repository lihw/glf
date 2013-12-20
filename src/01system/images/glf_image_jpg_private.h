// -------------------------------------------------------------- 
// glf_image_jpg_private.h
// Read and write JPEG image.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_IMAGE_JPG_PRIVATE_H
#define GLF_IMAGE_JPG_PRIVATE_H

#include "../../glf_common.h"

#include <cstdio>

GLF_EXTERN bool GLF_APIENTRY glfReadJPG(FILE* fp, GLuint& width, GLuint& height, GLuint& nchannels, GLubyte*& data);

// \param quality a 0-100 integer
GLF_EXTERN bool GLF_APIENTRY glfWriteJPG(FILE* fp, GLuint width, GLuint height, GLuint quality, GLuint nchannels, 
    const GLubyte* data); 


#endif // !GLF_IMAGE_JPG_PRIVATE_H

