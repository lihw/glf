// renderer.h
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#ifndef RENDERER_H
#define RENDERER_H

#include <glf/glf.h>

class Renderer : public GLFAbstractRenderer
{
public:
    virtual bool initialize();
    virtual void cleanup();
    virtual void render();

    virtual void onMouseButtonDown(int x, int y, int buttons, int modifiers);
    virtual void onMouseButtonUp(int x, int y, int buttons, int modifiers);
    virtual void onMouseMove(int x, int y, int buttons, int modifiers);

    virtual void onResized(int w, int h);

private:
    glf::Camera m_camera;
    //glf::Teapot m_teapot;
    glf::Shader m_shader;
    glf::Arcball m_arcball;
};

#endif // !RENDERER_H