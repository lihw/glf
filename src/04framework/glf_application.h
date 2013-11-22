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
extern GLFApplication* getApplication();
// Get the global main window.
extern GLFMainWindow* getMainWindow();

#endif // !GLF_APPLICATION_H
