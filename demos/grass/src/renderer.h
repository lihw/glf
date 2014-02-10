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
    void renderShadowMap();
    void generateAmbientOcclusion();

private:
    glf::RectColor*   m_background;
    glf::Drawable*    m_grid;
    glf::Drawable*    m_grass;
    glf::Drawable*    m_light;
    glf::RectTexture* m_shadowmap;
    glf::Box          m_grassBox;

    enum
    {
        GRID,
        STEM,
        NORMAL,
        TEXTURE_ONLY,
        PHONG,
        KAJIYA,
        SHADOWMAP,
        PHONG_TEXTURE,
        PHONG_TEXTURE_SSS,
        AO,
        AO_GENERATE,
        KAJIYA_TEXTURE,

        SHADER_FIRST  = GRID,
        SHADER_LAST   = KAJIYA_TEXTURE,
        SHADER_NUMBER = SHADER_LAST - SHADER_FIRST + 1,
    };
    
    glf::Shader     m_shaders[SHADER_NUMBER];

    struct 
    {
        GLfloat bladeHeight;
        GLfloat bladeWidth;
        GLfloat bladeThicknessThreshold;
    } m_geometrySetting;

    struct
    {
        bool                    showGeometryOnly;
        bool                    showNormal;

        GLuint                  lightingMode;
        glf::DirectionalLight   light;

        glf::Texture*           bladeTexture;
        bool                    useTexture;

        glf::Framebuffer*       smFramebuffer;
        bool                    smShow;
        GLuint                  smSize;
        bool                    translucency;

        bool                    ambientOcclusion;
    } m_renderingSetting;

    // We render the grass from x, y and z direction respectively. Each
    // rendering will write all fragments into a OIT UAV buffer with depth test
    // off. Later, for each fragment in the in the current camera, we can
    // compute its occlusion by computing the shadow term casted by neighboring
    // fragments in UAV buffers.
    enum 
    {
        GRID_XRES = 256,
        GRID_YRES = 256,
        DENSITY   = 32,
    };
    glf::ImageStorage*      m_splatTextureX1; // splat grid viewed at x coordinate.
    glf::ImageStorage*      m_splatTextureY1; // ditto.
    glf::ImageStorage*      m_splatTextureX2; // splat grid viewed at x coordinate.
    glf::ImageStorage*      m_splatTextureY2; // ditto.
    glf::ImageStorage*      m_splatTextureIndexX; // The index map viewed at x coordinate
    glf::ImageStorage*      m_splatTextureIndexY; // Ditto
    glf::Camera             m_aoCameras[2];
    glf::Atomic*            m_aoAtomic;
    GLuint*                 m_aoInitialIndex;
    glf::Rect*              m_aoRect;
    glf::Framebuffer*       m_aoFramebuffer;
    glf::RectTexture*       m_ao;

    glf::Camera     m_camera;
    glf::Arcball    m_arcball;
    glf::Arcball    m_arcballLight;
    bool            m_rotatingLight;
    GLint           m_width;
    GLint           m_height;
};

#endif // !RENDERER_H
