// renderer.h
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#ifndef RENDERER_H
#define RENDERER_H

#include <glf/glf.h>

#include "grassblade.h"


class Renderer : public GLFAbstractRenderer
{
    friend class Control;

public:
    Renderer();
    virtual ~Renderer();

    virtual bool initialize();
    virtual void cleanup();
    virtual void render();

    virtual void onKeyUp(int key, int modifiers);

    virtual void onMouseButtonDown(int x, int y, int buttons, int modifiers);
    virtual void onMouseButtonUp(int x, int y, int buttons, int modifiers);
    virtual void onMouseMove(int x, int y, int buttons, int modifiers);
    virtual void onMouseWheel(int numDegrees);

    virtual void onResized(int w, int h);

private:
    enum { NUM_BLADES = 10 };

    glf::Drawable*  m_grid;
    glf::Shader     m_gridShader;
    bool            m_showGrid;

    glf::RectColor* m_background;

    glf::Shader     m_grassBladeEditShader;
    glf::Shader     m_grassBladeKnotShader;
    glf::Shader     m_grassBladeNormalShader;
    glf::Shader     m_grassBladeViewShader;
    GrassBlade*     m_grassBlades[NUM_BLADES];
    glf::Texture*   m_grassBladeTexture;
    bool            m_showNormal;
    GLint           m_currentBladeIndex;
    GLint           m_currentBladeKnotIndex;

    enum
    {
        VIEW_MODE,
        EDIT_MODE,
    }              m_mode;


    bool           m_selfRotating;
    glf::Camera    m_editCamera;
    glf::Camera    m_viewCamera;
    glf::Camera    m_viewSelfRotatingCamera;

    
    glf::Arcball   m_arcball;
    GLfloat        m_rotation;
    GLint          m_width;
    GLint          m_height;

    GrassBlade*    m_editBlade;
};

#endif // !RENDERER_H
