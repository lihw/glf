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

#include "../glf_common.h"

class GLFAbstractControl;

class GLFControlWindow : public KxDockWidget
{
    Q_OBJECT

public:
    GLFControlWindow(QWidget* parent);
    virtual ~GLFControlWindow();

    GLFAbstractControl* getControl() const
    { return _control; }

private:
    GLFAbstractControl* _control;
    QWidget* _mainWidget;
};

#endif // !GLF_CONTROL_WINDOW_H

