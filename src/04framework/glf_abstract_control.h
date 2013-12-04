// -------------------------------------------------------------- 
// glf_abstract_control.h
// The container of controls and document model of the application.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_ABSTRACT_CONTROL_H
#define GLF_ABSTRACT_CONTROL_H

#include "../glf_common.h"

#include <QObject>

class GLFAbstractControl : public QObject
{
    Q_OBJECT

public:
    GLFAbstractControl(QWidget* parent);
    virtual ~GLFAbstractControl();

private:
    QWidget* _parent;
};

GLF_EXTERN GLFAbstractControl* glfCreateControls(QWidget* parent);

#endif // !GLF_ABSTRACT_CONTROL_H



