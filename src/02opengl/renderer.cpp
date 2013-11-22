//
// renderer.cpp
// Hongwei Li (lihw81@gmail.com)
//

#include "renderer.h"

Renderer* Renderer::_instance = NULL;

static void APIENTRY debugCallback(GLenum source,
                                   GLenum type,
                                   GLuint id,
                                   GLenum severity,
                                   GLsizei length,
                                   const GLchar* message,
                                   void* userParam)
{
    char debSource[32], debType[32], debSev[32];

    bool Error(false);

    if(source == GL_DEBUG_SOURCE_API_ARB)
    {
        strcpy(debSource, "OpenGL");
    }
    else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
    {
        strcpy(debSource, "Windows");
    }
    else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
    {
        strcpy(debSource, "Shader Compiler");
    }
    else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
    {
        strcpy(debSource, "Third Party");
    }
    else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)
    {
        strcpy(debSource, "Application");
    }
    else if(source == GL_DEBUG_SOURCE_OTHER_ARB)
    {
        strcpy(debSource, "Other");
    }

    if(type == GL_DEBUG_TYPE_ERROR_ARB)
    {
        strcpy(debType, "error");
    }
    else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
    {
        strcpy(debType, "deprecated behavior");
    }
    else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
    {
        strcpy(debType, "undefined behavior");
    }
    else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)
    {
        strcpy(debType, "portability");
    }
    else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
    {
        strcpy(debType, "performance");
    }
    else if(type == GL_DEBUG_TYPE_OTHER_ARB)
    {
        strcpy(debType, "message");
    }

    if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)
    {
        strcpy(debSev, "high");
        Error = true;
    }
    else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
    {
        strcpy(debSev, "medium");
    }
    else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)
    {
        strcpy(debSev, "low");
    }

#if defined _DEBUG
    glusLogPrint(GLUS_LOG_ERROR, "OpenGL error: %s: %s(%s) %d: %s", debSource, debType, debSev, id, message);
#endif 
}


Renderer* Renderer::instance()
{
    if (_instance == NULL)
    {
        _instance = new Renderer();
    }

    return _instance;
}

Renderer::Renderer()
{
    // Register the debug output function.
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageCallbackARB(debugCallback, this);
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM_ARB, 0, NULL, GL_TRUE);
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH_ARB, 0, NULL, GL_TRUE);
}

void Renderer::checkOpenGLError(const GLUSchar* message)
{
    GLUSenum error;

    error = glGetError();

    if (error == GL_NO_ERROR)
    {
        return ;
    }

    GLUSint errcode;
    static GLUSchar* errorOpenGLMessages[] = 
    {
        "Invalid enumerant", 
        "Invalid operation",
        "Invalid value"
    };

    switch (error)
    {
    case GL_INVALID_ENUM:
        errcode = 0;        
        break;
    case GL_INVALID_OPERATION:
        errcode = 1;
        break;
    case GL_INVALID_VALUE:
        errcode = 2;
        break;
    }

    glusLogPrint(GLUS_LOG_ERROR, "%s: %s", message, errorOpenGLMessages[errcode]);

#if defined _DEBUG
    // FIXME:
    // abort at this call stack.
#endif
}

void Renderer::apply()
{
}
