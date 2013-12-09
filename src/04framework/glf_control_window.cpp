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

#include "glf_abstract_control.h"

#include "../01system/glf_assert.h"

GLFControlWindow::GLFControlWindow(QWidget* parent)
    : KxDockWidget(tr("Control"), parent)
{
    setFloating(true);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget:: DockWidgetFloatable);
    setAllowedAreas(Qt::NoDockWidgetArea);
    setMinimumSize(120, 480);

    _mainWidget = new QWidget(this);
    setWidget(_mainWidget);

    _control = glfCreateControls(_mainWidget);
    GLF_ASSERT(_control != NULL);
}

GLFControlWindow::~GLFControlWindow()
{
    delete _control;
}
