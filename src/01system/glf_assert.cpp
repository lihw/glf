// -------------------------------------------------------------- 
// glf_assert.cpp
// Assert
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_assert.h" 

#include "glf_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <dbghelp.h>

GLF_NAMESPACE_BEGIN

AssertStyleEnum g_assertStyle = GLF_ASSERT_STYLE_DEFAULT;

void GLF_APIENTRY assertFunction(const char* condition, const char* file, GLuint line, const AssertStyleEnum style)
{
    static char buffer[1024];

    switch (style) 
    {
        case GLF_ASSERT_STYLE_LOGANDEXIT:
            GLF_LOGINFO("Assert: %s!", condition);
            abort();
            break;
        case GLF_ASSERT_STYLE_DEBUG_BREAK:
            sprintf_s(buffer, 1024, "Assert: %s\n", condition);
            OutputDebugStringA(buffer);
            OutputDebugStringA("\n");
            __debugbreak();
            break;
        case GLF_ASSERT_STYLE_DEBUG_INFO:                                                                              
            sprintf_s(buffer, 1024, "Assert: %s\nPosition: %s(%d)\n", condition, file, line);
            OutputDebugStringA(buffer);
            OutputDebugStringA("\n");
            break;
        case GLF_ASSERT_STYLE_POPUP_BOX:
            sprintf_s(buffer, 1024, "Assert: %s\nPosition: %s(%d)\n", condition, file, line);
            MessageBoxA(NULL, buffer, "Error", MB_YESNO | MB_ICONERROR | MB_TASKMODAL);
            break;
        default:
            // ??
            break;
    }
}

void GLF_APIENTRY assertSetStyle(AssertStyleEnum style)
{
    g_assertStyle = style;
}

GLF_NAMESPACE_END
