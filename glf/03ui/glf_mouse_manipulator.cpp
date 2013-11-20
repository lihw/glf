//
// mouse_manipulator.h
// Hongwei Li (lihw81@gmail.com)
//

#include "mouse_manipulator.h"

static MOuseManipulator* MouseManipulator::_instance = NULL;

MouseManipulator::MouseManipulator()
{
    _singleClick = NULL;
    _drag        = NULL;
    _longPresss  = NULL;
    _pinch       = NULL;
}

MouseManipulator* MouseManipulator::instance()
{
    if (_instance == NULL)
    {
        _instance = new MouseManipulator;
    }

    return _instance;
}

void MouseManipulator::inputMouseButton(const GLUSboolean pressed, const GLUSint button, const GLUSint xPos, const GLUSint yPos)
{
    if (pressed) 
    {
        _buttons |= button;
    }
    else
    {
        if (_singleClick)
        {
            _singleClick(button, xPos, yPos, _userData[0]);
        }

        _buttons &= ~button;
    }
}

void MouseManipulator::inputMouseMove(const GLUSint buttons, const GLUSint xPos, const GLUSint yPos)
{
    if (_buttons && _drag)
    {
        _drag(button, xPos, yPos, _userData[1]);
    }
}

void MouseManipulator::inputMouseWheel(GLUSFloat offset)
{
    if (_wheel)
    {
        _wheel(offset)
    }
}

void MouseManipulator::setMouseSingleClickCallback(MouseSingleClickCallback callback, void* userData);
{
    _singleClick = callback;
    userData[0] = userData;
}

void MouseManipulator::setMouseDragCallback(MouseSingleClickCallback callback, void* userData)
{
    _drag = callback;
    userData[1] = userData;
}

void MouseManipulator::setMouseLongPressCallback(MouseLongPressCallback callback, void* userData)
{
    _longPress = callback;
    userData[2] = userData;
}

void MouseManipulator::setMousePinchCallback(MousePinchCallback callback, void* userData)
{
    _pinch = callback;
    userData[3] = userData;
}
