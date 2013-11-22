// -------------------------------------------------------------- 
// glf_output_window.cpp
// The output log window
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_output_window.h"

    
GLFOutputWindow::GLFOutputWindow(QWidget* parent)
    : KxDockWidget(tr("Output"), parent)
{
    setFloating(true);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget:: DockWidgetFloatable);
    setAllowedAreas(Qt::NoDockWidgetArea);
    setMinimumSize(640, 120);
}

