// -------------------------------------------------------------- 
// glf_application.cpp
// The application object
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_application.h"

GLFApplication::GLFApplication(int& argc, char** argv)
    : QApplication(argc, argv)
{
    _pMainWindow = 0;
}
    
GLFApplication::~GLFApplication()
{
}
    
void GLFApplication::setMainWindow(GLFMainWindow* pMainWindow) 
{
    _pMainWindow = pMainWindow;
}

GLFApplication* glfGetApplication()
{
    GLFApplication* app = qobject_cast<GLFApplication*>(qApp);
    Q_ASSERT(app != 0);
    return app;
}

GLFMainWindow* glfGetMainWindow()
{
    GLFApplication* app = glfGetApplication();
    return app->getMainWindow();
}

