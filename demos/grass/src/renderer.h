// renderer.h
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#ifndef RENDERER_H
#define RENDERER_H

#include <glf/glf.h>

class GrassAsset;

class Renderer : public GLFAbstractRenderer
{
    friend class Control;

public:
    Renderer();
    virtual ~Renderer();

    virtual bool initialize();
    virtual void cleanup();
    virtual void render();

    virtual void onMouseButtonDown(int x, int y, int buttons, int modifiers);
    virtual void onMouseButtonUp(int x, int y, int buttons, int modifiers);
    virtual void onMouseMove(int x, int y, int buttons, int modifiers);
    virtual void onMouseWheel(int numDegrees);

    virtual void onResized(int w, int h);

private:
    glf::RectColor* m_background;

    glf::Drawable*  m_grid;
    glf::Shader     m_gridShader;

    enum
    {
        STEM_ONLY,
        COLOR,

        GRASS_SHADER_FIRST = STEM_ONLY,
        GRASS_SHADER_LAST = COLOR,
        GRASS_SHADER_NUMBER = GRASS_SHADER_LAST - GRASS_SHADER_FIRST + 1,
    };
    glf::Drawable*  m_grass;
    glf::Shader     m_grassShaders[GRASS_SHADER_NUMBER];
    glf::Shader*    m_currentGrassShader;
    bool            m_grassShowNormal;
    glf::Shader     m_grassNormalShader;

    glf::Camera     m_camera;
    glf::Arcball    m_arcball;
    GLint           m_width;
    GLint           m_height;
};

#endif // !RENDERER_H
