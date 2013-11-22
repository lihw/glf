// -------------------------------------------------------------- 
// glf_widget.cpp
// The base class of all widgets 
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_widget.h"

GLF_NAMESPACE_BEGIN


Widget::Widget(const char* name)
    , _name(name)
{
}

Widget::~Widget()
{
}

void Widget::setVisible(bool visible)
{
    _visible = visible;
}

void Widget::setEnabled(bool enabled)
{
    _enabled = enabled;
}

void Widget::setFgColor(GLubyte r, GLubyte g, GLubyte b)
{
    _fgColor[0] = r;
    _fgColor[1] = g;
    _fgColor[2] = b;
}

void Widget::setBgColor(GLubyte r, GLubyte g, GLubyte b)
{
    _bgColor[0] = r;
    _bgColor[1] = g;
    _bgColor[2] = b;
}

void Widget::setCallback(void* value, WidgetCallback callback)
{
    _callback = callback;
    _value = value;
}

GLF_NAMESPACE_END
