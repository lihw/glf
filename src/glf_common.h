// -------------------------------------------------------------- 
// glf_common.h
// The common stuff about glf
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#ifndef GLF_COMMON_H
#define GLF_COMMON_H

#include <windows.h>

// -------------------------------------------------------------- 
// GL
// -------------------------------------------------------------- 
#define GLEW_STATIC
#include <GL/glew.h>


// -------------------------------------------------------------- 
// GLM
// -------------------------------------------------------------- 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/euler_angles.hpp>


// -------------------------------------------------------------- 
// GLF
// -------------------------------------------------------------- 
#define GLF_NAMESPACE_BEGIN namespace glf {
#define GLF_NAMESPACE_END   };

#if defined _DEBUG || defined DEBUG
# define GLF_DEBUG 1
#endif

#define GLF_UNUSED_ARGUMENT(p) ((void)p)

#if defined _MSC_VER || defined _WIN32
# define GLF_WIN32
#else
# error "GLFramework supports win32 only"
#endif

#if defined GLF_WIN32

#define GLF_DLLEXPORT 
#define GLF_INLINE __forceinline
#define GLF_EXTERN extern
#define GLF_CALLBACK
#define GLF_APIENTRY __stdcall
#define GLF_CCONV __cdecl

#endif // GLF_WIN32

// -------------------------------------------------------------- 
// Misc
// -------------------------------------------------------------- 
#ifndef GLF_PI
# define GLF_PI 3.1415926535897932384626f
#endif // !GLF_PI


#endif // !GLF_COMMON_H
