// -------------------------------------------------------------- 
// glf_abstract_renderer.h
// an empty renderer which will be the base class of user defined
// renderer.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_ABSTRACT_RENDERER_H
#define GLF_ABSTRACT_RENDERER_H

#include "../glf_common.h"

class GLFAbstractRenderer 
{
public:
    GLFAbstractRenderer();
    virtual ~GLFAbstractRenderer();

    virtual bool initialize();
    virtual void cleanup() = 0;
    virtual void render() = 0;

    enum 
    {
        MOUSE_BUTTON1 = 0x1,
        MOUSE_BUTTON2 = 0x2,
        MOUSE_BUTTON3 = 0x4,
        KEYBOARD_CTRL = 0x8,
        KEYBOARD_ALT  = 0x10,
        KEYBOARD_SHIFT = 0x20,
    };
    
    virtual void onMouseButtonDown(int x, int y, int buttons, int modifiers);
    virtual void onMouseButtonUp(int x, int y, int buttons, int modifiers);
    virtual void onMouseMove(int x, int y, int buttons, int modifiers);
    virtual void onMouseWheel(int numDegrees);

    virtual void onKeyDown(int key, int modifiers);
    virtual void onKeyUp(int key, int modifiers);

    virtual void onResized(int w, int h);
};

GLF_EXTERN GLFAbstractRenderer* glfCreateRenderer();

#endif // !GLF_ABSTRACT_RENDERER_H



