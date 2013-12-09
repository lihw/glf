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

#include "glf_abstract_control.h"

GLFAbstractControl::GLFAbstractControl(QWidget* parent)
    : QObject()
{
    Q_ASSERT(parent != NULL);
    _parent = parent;
}

GLFAbstractControl::~GLFAbstractControl()
{
}

