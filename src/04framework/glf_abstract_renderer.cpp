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
