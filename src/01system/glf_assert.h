// -------------------------------------------------------------- 
// glf_assert.h
// Assert
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_ASSERT_H
#define GLF_ASSERT_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

#define GLF_COMPILE_TIME_ASSERT(pred) switch(0) { case 0: case (pred): break; }

// The styles of the assert.
enum AssertStyleEnum
{
    GLF_ASSERT_STYLE_LOGANDEXIT,     ///< write to stdout/logcat and exit.
    GLF_ASSERT_STYLE_POPUP_BOX,      ///< use popup dialog box.
    GLF_ASSERT_STYLE_DEBUG_BREAK,    ///< OutputDebugString + debug break
    GLF_ASSERT_STYLE_DEBUG_INFO,     ///< outputDebugString

    GLF_ASSERT_STYLE_DEFAULT = GLF_ASSERT_STYLE_DEBUG_BREAK,
};

//
// Assert function.
// \param condition the condition string.
// \param file in which file this assert is triggered.
// \param line at which line this assert is.
// \param style how to present this assert to user.
GLF_EXTERN void GLF_APIENTRY assertFunction(const char* condition, const char* file, GLuint line, 
        const AssertStyleEnum style);

GLF_EXTERN AssertStyleEnum g_assertStyle;

// Set the assert style in the application-wise scope. The default style is GLF_ASSERT_STYLE_DEBUG_BREAK.
GLF_EXTERN void GLF_APIENTRY assertSetStyle(AssertStyleEnum style);

#if defined GLF_DEBUG

#define GLF_ASSERT(condition) \
{ \
    if (!(condition)) \
    { \
        glf::assertFunction((#condition), __FILE__, __LINE__, glf::g_assertStyle); \
    } \
}

#define GLF_ASSERT_INFO(condition, text) \
{ \
    if (!(condition)) \
    { \
        glf::assertFunction(text, __FILE__, __LINE__, glf::g_assertStyle); \
    } \
}

#define GLF_ASSERT_NOT_REACHABLE() \
    glf::assertFunction("should not reach here", __FILE__, __LINE__, glf::g_assertStyle); 

#define GLF_ASSERT_NOT_IMPLEMENTED() \
    glf::assertFunction("not implemented", __FILE__, __LINE__, glf::g_assertStyle); 

#else

#define GLF_ASSERT(condition)
#define GLF_ASSERT_INFO(condition, text)
#define GLF_ASSERT_NOT_REACHABLE() 
#define GLF_ASSERT_NOT_IMPLEMENTED() 

#endif // GLF_DEBUG

GLF_NAMESPACE_END


#endif // !GLF_ASSERT_H
