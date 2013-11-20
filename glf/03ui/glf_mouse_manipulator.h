//
// mouse_manipulator.h
// Hongwei Li (lihw81@gmail.com)
//
// Translate the preliminary mouse events into high-level events.

#include <GLUS/glus.h>


#ifndef MOUSE_MANIPULATOR_H
#define MOUSE_MANIPULATOR_H


typedef void (*MouseSingleClickCallback)(GLUSint button, GLUSint xPos, GLUSint yPos, void* userData);
typedef void (*MouseDragCallback)(GLUSint button, GLUSint xPos, GLUSint yPos, void* userData);
typedef void (*MouseLongPressCallback)(GLUSint button, GLUSint xPos, GLUSint yPos, void* userData);
typedef void (*MousePinchCallback)(GLUSint button, GLUSfloat offsetx, GLUSfloat offsety, void* userData);
typedef void (*MouseWheelCallback)(GLUint ticks, void* userData);

class MouseManipulator
{
private:
    // Singleton. No public default constructor.
    MouseManipulator();
    // No public copy constructor.
    MouseManipulator(const MouseManipulator& manipulator) {};

public:
    static MouseManipulator* instance();

    // The preliminary mouse message from system. The button up/down event.
    void inputMouseButton(const GLUSboolean pressed, const GLUSint button, const GLUSint xPos, const GLUSint yPos);
    // The mouse moving message from system.
    void inputMouseMove(const GLUSint buttons, const GLUSint xPos, const GLUSint yPos);
    // The mouse wheel rolling message from system.
    void inputMouseWheel(const GLUSint buttons, const GLUSint ticks, const GLUSint xPos, const GLUSint yPos);

    // Set single click callback.
    void setMouseSingleClickCallback(MouseSingleClickCallback callback);

    // Set drag callback.
    void setMouseDragCallback(MouseSingleClickCallback callback);

    // Set long press callback.
    void setMouseLongPressCallback(MouseLongPressCallback callback);

    // Set pinch callback
    void setMousePinchCallback(MousePinchCallback callback);
private:

    static MouseManipulator* _instance;

    MouseSingleClickCallback _singleClick;
    MouseDragCallback        _drag;
    MouseLongPressCallback   _longPress;
    MousePinchCallback       _pinch;

    void* _userData[32];

    GLUSuint _buttons; // The button pressed.
};


#endif
