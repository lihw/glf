// vao.cpp
//
// Created at 2013/11/25
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
    config->windowWidth = 1024;
    config->windowHeight = 768;

    config->noOutputWindow = true;
}

GLFAbstractRenderer* glfCreateRenderer()
{
    return new Renderer();
}
