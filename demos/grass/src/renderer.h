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

    void setBladeHeight(GLfloat height);

private:
    bool loadShaders();

private:
    glf::RectColor* m_background;
    glf::Drawable*  m_grid;
    glf::Drawable*  m_grass;

    enum
    {
        GRID,
        STEM,
        NORMAL,
        TEXTURE_ONLY,
        PHONG,
        KAJIYA,
        PHONG_TEXTURE,
        KAJIYA_TEXTURE,

        SHADER_FIRST  = GRID,
        SHADER_LAST   = KAJIYA_TEXTURE,
        SHADER_NUMBER = SHADER_LAST - SHADER_FIRST + 1,
    };
    
    glf::Shader     m_shaders[SHADER_NUMBER];

    struct 
    {
        GLfloat bladeWidth;
        GLfloat bladeThicknessThreshold;
    } m_geometrySetting;

    struct
    {
        bool              showGeometryOnly;
        bool              showNormal;

        GLuint            lightingMode;
        glf::PointLight   pointLight;

        glf::Texture*     bladeTexture;
        bool              useTexture;
    } m_renderingSetting;


    glf::Camera     m_camera;
    glf::Arcball    m_arcball;
    GLint           m_width;
    GLint           m_height;
};

#endif // !RENDERER_H
