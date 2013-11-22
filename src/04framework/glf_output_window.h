// -------------------------------------------------------------- 
// glf_output_window.h
// The output log window
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_OUTPUT_WINDOW_H
#define GLF_OUTPUT_WINDOW_H

#include <QDockWidget>

#include "controls/widgets/window/KxDockWidget.h"

class GLFOutputWindow : public KxDockWidget
{
    Q_OBJECT

public:
    GLFOutputWindow(QWidget* parent);
};

#endif 

