// renderer.cpp
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "renderer.h"


bool Renderer::initialize()
{
    if (!m_shader.loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    m_shader.getUniform("Color")->setValue(1.0f, 1.0f, 0.0f, 1.0f);

    glClearColor(1, 0, 0, 0);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    const char* meshFile = "../data/models/teapot.obj";
    try
    {
        m_mesh = new glf::Mesh(meshFile);
    }
    catch (std::string err)
    {
        GLF_LOGERROR("Failed to load %s", meshFile);
        return false;
    }

    m_camera.lookAt(0, 0, 10.0f,
                    0, 0, 0,
                    0, 1, 0);

    return true;
}

void Renderer::cleanup()
{
    delete m_mesh;
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.getUniform("MVP")->setValue(m_camera.getProjectionModelviewMatrix());
    m_shader.enable();

    m_mesh->render(1);
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
        GLfloat cx, cy;
        cx = (GLfloat)x / (GLfloat)m_width;
        cy = ((GLfloat)m_height - 1.0f - (GLfloat)y) / (GLfloat)m_height;
        
        m_arcball.updateMouse(cx, cy);
        m_camera.setRotation(m_arcball.getRotationMatrix());
    }       
}

void Renderer::onResized(int w, int h)
{
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    m_camera.setPerspective(45.0f, aspect, 0.1f, 1000.0f);
    //m_camera.setOrthogonal(-1.0f, 1.0f, -1.0f, 1.0f, 0, 100.0f);
    glViewport(0, 0, w, h);

    m_width = w;
    m_height = h;
}
