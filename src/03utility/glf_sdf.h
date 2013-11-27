// -------------------------------------------------------------- 
// glf_sdf.h
// The signed distance field computation
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_SDF_H
#define GLF_SDF_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

// Brute force 
GLF_EXTERN GLubyte* createSDFBruteForce(const Bitmap& bitmap, GLuint outputWidth, GLuint outputHeight);

// http://www.codersnotes.com/notes/signed-distance-fields
GLF_EXTERN GLubyte* createSDF8SSEDT(const Bitmap& bitmap, GLuint outputWidth, GLuint outputHeight);


GLF_NAMESPACE_END


#endif // !GLF_SDF_H
