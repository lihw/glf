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

GLF_NAMESPACE_BEGIN

typedef void (*WidgetCallback)(void* value);

class Widget 
{
    friend class Widget_Master;

public:
    Widget(Widget* parent = NULL);
    virtual ~Widget();

    GLF_INLINE Widget* parent() { return m_parent; };
    void setParent(Widget* parent);

public:
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

    virtual void onMouseOver(int x, int y);
    virtual bool onMouseButton(int button, int action, int mods);
    virtual bool onMouseMove(int x, int y);

    void setCallback(void* value, WidgetCallback callback = 0);

protected:
    int   _height;    // The widget height
    int   _width;    // The widget width
    int   _x;    // The widget left bottom corner x;
    int   _y;    // The widget left bottom corner y;

    char _name[1024];      // The label of widget

    Widget* _parent;    // The parent
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
