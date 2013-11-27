// renderer.cpp
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "renderer.h"


bool Renderer::initialize()
{
    if (!m_shader.loadFromLibrary(glf::Shader::COLOR, glf::Shader::COLOR))
    {
        return false;
    }
    m_shader.getUniform("Color")->setValue(1.0f, 0.0f, 0.0f, 1.0f);

    glDepthFunc(GL_LEQUAL);

    return true;
}

void Renderer::cleanup()
{
}

void Renderer::render()
{
    m_shader.getUniform("MVP")->setValue(m_camera.getProjectionModelviewMatrix());
    m_shader.enable();

    //m_grid.draw();
    m_shader.disable();
}

void Renderer::onMouseButtonDown(int x, int y, int buttons, int modifiers)
{
    if (buttons == MOUSE_BUTTON1)
    {
        m_arcball.restart();
    }
}

void Renderer::onMouseButtonUp(int x, int y, int buttons, int modifiers)
{
}

void Renderer::onMouseMove(int x, int y, int buttons, int modifiers)
{
    if (buttons == MOUSE_BUTTON1)
    {
        m_arcball.updateMouse(x, y);
        m_camera.setRotation(m_arcball.getRotationMatrix());
    }       
}

void Renderer::onResized(int w, int h)
{
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    m_camera.setPerspective(45.0f, aspect, 0.1f, 1000.0f);
}
