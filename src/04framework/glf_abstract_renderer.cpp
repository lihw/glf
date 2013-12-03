// -------------------------------------------------------------- 
// glf_abstract_renderer.cpp
// an empty renderer which will be the base class of user defined
// renderer.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_abstract_renderer.h"

GLFAbstractRenderer::GLFAbstractRenderer()
{
}

GLFAbstractRenderer::~GLFAbstractRenderer()
{
}

bool GLFAbstractRenderer::initialize()
{
    return true;
}
    
void GLFAbstractRenderer::onMouseButtonDown(int x, int y, int buttons, int modifiers)
{
}

void GLFAbstractRenderer::onMouseButtonUp(int x, int y, int buttons, int modifiers)
{
}

void GLFAbstractRenderer::onMouseMove(int x, int y, int buttons, int modifiers)
{
}

void GLFAbstractRenderer::onMouseWheel(int numDegrees)
{
}

void GLFAbstractRenderer::onKeyDown(int key, int modifiers)
{
}

void GLFAbstractRenderer::onKeyUp(int key, int modifiers)
{
}

void GLFAbstractRenderer::onResized(int w, int h)
{
}
