// -------------------------------------------------------------- 
// glf_control_window.cpp
// The output log window
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_control_window.h"

GLFControlWindow::GLFControlWindow(QWidget* parent)
    : KxDockWidget(tr("Control"), parent)
{
    setFloating(true);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget:: DockWidgetFloatable);
    setAllowedAreas(Qt::NoDockWidgetArea);
    setMinimumSize(120, 480);
}

