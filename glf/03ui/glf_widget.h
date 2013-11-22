// -------------------------------------------------------------- 
// glf_widget.h
// The base class of all widgets 
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_BFONT_H
#define GLF_BFONT_H

#include "../glf_common.h"

#include <string>

GLF_NAMESPACE_BEGIN

typedef void (*WidgetCallback)(void* value);

class Widget 
{
public:
    Widget(const char* name);
    virtual ~Widget();

    void setPosition(GLint x, GLint y);
    void setSize(GLint width, GLint height);

    void setVisible(bool visible);
    void setEnabled(bool enabled);
    GLF_INLINE bool isVisible() const { return _visible; };
    GLF_INLINE bool isEnabled() const { return _enabled; };
    
    const char* getName() const { return _name; };
    
    void setFgColor(GLubyte r, GLubyte g, GLubyte b);
    void setBgColor(GLubyte r, GLubyte g, GLubyte b);

    virtual void render() = 0;

    virtual void onMouseOver(GLint x, GLint y);
    virtual bool onMouseButton(GLint button, GLint action, GLint mods);
    virtual bool onMouseMove(GLint x, GLint y);

    void setCallback(void* value, WidgetCallback callback = 0);

protected:
    int   _height;    // The widget height
    int   _width;    // The widget width
    int   _x;    // The widget left bottom corner x;
    int   _y;    // The widget left bottom corner y;

    std::string _name;      // The label of widget

    bool    _visible;   // Visible or hidden
    bool    _enabled;   // functional or disabled

    GLubyte _fgColor[3];
    GLubyte _bgColor[3];

    WidgetCallback _callback;  // The widget's callback
    void*          _value; // The callback value

    bool       _clicked; // The widget has been currently clicked
    bool       _hover; // The widget is ccurently touched by mouse
};


GLF_NAMESPACE_END

#endif // GLF_WIDGET
