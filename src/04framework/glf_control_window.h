// -------------------------------------------------------------- 
// glf_control_window.h
// The control panel 
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_CONTROL_WINDOW_H
#define GLF_CONTROL_WINDOW_H

#include "controls/widgets/window/KxDockWidget.h"

class GLFControlWindow : public KxDockWidget
{
    Q_OBJECT

public:
    GLFControlWindow(QWidget* parent);
};

#endif // !GLF_CONTROL_WINDOW_H

