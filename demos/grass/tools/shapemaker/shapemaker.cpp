// shapemaker.cpp
//
// Created at 2013/12/11
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include <glf/glf.h>

#include "renderer.h"
#include "control.h"

GLFAbstractControl* glfCreateControls(QWidget* parent)
{
    return new Control(parent);
}

void glfGenerateConfig(GLFConfig* config)
{
    config->noOutputWindow = true;

    config->windowWidth = 768;
    config->windowHeight = 768;
}

GLFAbstractRenderer* glfCreateRenderer()
{
    return new Renderer();
}
