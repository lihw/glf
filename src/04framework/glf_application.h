// -------------------------------------------------------------- 
// glf_application.h
// The application object
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_APPLICATION_H
#define GLF_APPLICATION_H 

#include <QApplication>

class GLFMainWindow;
class GLFAbstractRenderer;
class GLFAbstractControl;

class GLFApplication : public QApplication
{
    Q_OBJECT

public:
    GLFApplication(int& argc, char** argv);
    ~GLFApplication();

    void setMainWindow(GLFMainWindow* pMainWindow);
    GLFMainWindow* getMainWindow() const
    { return _pMainWindow; }

private:
    GLFMainWindow* _pMainWindow;
};

// Get the global application instance.
extern GLFApplication* glfGetApplication();
// Get the global main window.
extern GLFMainWindow* glfGetMainWindow();
// Get the global renderer
extern GLFAbstractRenderer* glfGetRenderer();
// Get the global control
extern GLFAbstractControl* glfGetControl();
// Print the message to the output window
extern void glfPrintLogMessage(const char* message);

#endif // !GLF_APPLICATION_H
