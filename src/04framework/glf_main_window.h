// -------------------------------------------------------------- 
// glf_main_window.h
// The main window
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_MAIN_WINDOW_H
#define GLF_MAIN_WINDOW_H

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "glf_config.h"

class GLFRenderWidget;
class GLFOutputWindow;
class GLFControlWindow;

class GLFMainWindow : public QMainWindow
{	
    Q_OBJECT

public:
    // Constructor.
    GLFMainWindow(const GLFConfig& config, QWidget* parent = 0);

    // Interface
    GLFRenderWidget* getRenderWidget() const
    { return _pRenderWidget; }
    GLFOutputWindow* getOutputWindow() const
    { return _pOutputWindow; }
    GLFControlWindow* getControlWindow() const
    { return _pControlWindow; }

public slots:
    void onDumpScreen();
    void onAbout();

protected:
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);

private:
    GLFRenderWidget*    _pRenderWidget;
    GLFOutputWindow*    _pOutputWindow;
    GLFControlWindow*   _pControlWindow;
    GLFConfig           _config;

    QMenu*    _pFileMenu;
    QAction*  _pAboutAct;
    QAction*  _pDumpAct;
};


#endif // !GLF_MAIN_WINDOW_H
