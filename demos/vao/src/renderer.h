// renderer.h
//
// Created at 2014/2/12
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#ifndef RENDERER_H
#define RENDERER_H

#include <glf/glf.h>

class Scene;

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
    bool loadShaders();
    void generateAmbientOcclusionMap();

private:
    glf::RectColor*   m_background;
    glf::Drawable*    m_grid;
    Scene*            m_scene;
    glf::Shader       m_gridShader;
    glf::Shader       m_sceneShader;

    glf::Camera     m_camera;
    glf::Arcball    m_arcball;
    GLint           m_width;
    GLint           m_height;

    struct
    {
        bool     aoEnabled;
        GLuint   aoSize;
        GLuint   aoNumLayers;
    } m_setting;

    glf::Shader             m_aoGenerateShader;
    glf::Shader             m_aoBlendShader;
    glf::Shader             m_aoRenderShader;
    glf::ImageStorage*      m_aoFragmentTexture;      // splate texture
    glf::ImageStorage*      m_aoStartOffsetTexture; // the first index
    glf::Camera             m_aoCamera;
    glf::Atomic*            m_aoAtomic;
    glf::Rect*              m_aoClearRect;
    glf::RectTexture*       m_aoDebugRect;
    glf::Rect*              m_aoBlendRect;
    glf::Framebuffer*       m_aoFramebuffer;

};

#endif // !RENDERER_H
