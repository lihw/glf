// renderer.cpp
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "renderer.h"


bool Renderer::initialize()
{
    glm::mat4 mvMat = glm::lookAt(glm::vec3(0.0f, 0.1f, 5.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projMat  = glm::perspective(45.0f, 1.3f, 0.1f, 1000.0f);
    glm::vec4 p0 = glm::vec4(-1.0f, 0.0f, -1.0f, 1.0f);
    glm::vec4 p1 = glm::vec4(-1.0f, 0.0f,  1.0f, 1.0f);

    glm::vec4 proj0 = projMat * mvMat * p0;
    glm::vec4 proj1 = projMat * mvMat * p1;

    proj0.x /= proj0.w;
    proj0.y /= proj0.w;
    proj1.x /= proj1.w;
    proj1.y /= proj1.w;

    glm::vec2 pt0 = (glm::vec2(proj0.x, proj0.y) * 0.5f + glm::vec2(0.5f, 0.5f)) * glm::vec2(1024.0f, 768.0f);
    glm::vec2 pt1 = (glm::vec2(proj1.x, proj1.y) * 0.5f + glm::vec2(0.5f, 0.5f)) * glm::vec2(1024.0f, 768.0f);

    float d = glm::length(pt0 - pt1);

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
        m_teapot = new glf::Drawable(meshFile);
        m_plane = new glf::Drawable(new glf::Quad());
    }
    catch (std::string err)
    {
        GLF_LOGERROR("Failed to load %s", meshFile);
        return false;
    }

    m_model = m_teapot;

    m_camera.lookAt(0, 0.1f, 5.0f,
                    0, 0, 0,
                    0, 1, 0);

    return true;
}

void Renderer::cleanup()
{
    delete m_teapot;
    delete m_plane;
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 mat = m_camera.getProjectionModelviewMatrix() * m_teapot->getTransformation();

    m_shader.getUniform("MVP")->setValue(mat);
    m_shader.enable();

    m_model->render(1);
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

void Renderer::onMouseWheel(int numDegrees)
{
    m_camera.translate(0, 0, (GLfloat)numDegrees * 0.1f);
}

void Renderer::onMouseMove(int x, int y, int buttons, int modifiers)
{
    if (buttons == MOUSE_BUTTON1)
    {
        GLfloat cx, cy;
        cx = (GLfloat)x / (GLfloat)m_width;
        cy = ((GLfloat)m_height - 1.0f - (GLfloat)y) / (GLfloat)m_height;
        
        m_arcball.updateMouse(cx, cy);
        m_camera.setRotation(&(m_arcball.getRotationMatrix()[0][0]));
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
    
void Renderer::setModel(GLuint model)
{
    if (model == 0)
    {
        m_model = m_teapot;
    }
    else if (model == 1)
    {
        m_model = m_plane;
    }
    else
    {
        GLF_LOGERROR("Unknown model");
        m_model = m_teapot;
    }
}
