// renderer.cpp
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "renderer.h"

#include "grass_asset.h"

Renderer::Renderer()
    : GLFAbstractRenderer()
{
    m_background = NULL;
    m_grid = NULL;
    m_grass = NULL;
}

Renderer::~Renderer()
{
}

bool Renderer::initialize()
{
    glClearColor(1, 0, 0, 0);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    m_camera.lookAt(0, 0, 10.0f,
                    0, 0, 0,
                    0, 1, 0);

    // -------------------------------------------------------------- 
    // Initialize shaders
    // -------------------------------------------------------------- 
    if (!m_grassShaders[STEM_ONLY].loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    m_grassShaders[STEM_ONLY].getUniform("Color")->setValue(1.0f, 1.0f, 0.0f, 1.0f);

#define PATH_PREFIX "../demos/grass/data/shader"
    if (!m_grassShaders[COLOR].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/grass_color.fs", NULL, NULL, 
        PATH_PREFIX"/grass.gs"))
    {
        return false;
    }
    m_grassShaders[COLOR].getUniform("Color")->setValue(1.0f, 1.0f, 0.0f, 1.0f);
    m_grassShaders[COLOR].getUniform("BladeWidth")->setValue(0.01f);
#undef PATH_PREFIX

    m_currentGrassShader = &m_grassShaders[STEM_ONLY];


    // -------------------------------------------------------------- 
    // Load/create the grass asset
    // -------------------------------------------------------------- 
    GrassAsset* grassAsset = new GrassAsset();
    glf::Mesh* grassMesh = new glf::Mesh(grassAsset->getVertices(),
                                         grassAsset->getNumVertices(),
                                         grassAsset->getIndices(),
                                         grassAsset->getNumIndices(),
                                         grassAsset->getPrimitive(),
                                         grassAsset->getVertexDesc(),
                                         grassAsset->getNumVertexDescEntries());
    m_grass = new glf::Drawable(grassMesh);
    delete grassAsset;

    
    // -------------------------------------------------------------- 
    // Init background
    // -------------------------------------------------------------- 
    const GLfloat backgroundColor[] =
    {
        0.5f, 0.5f, 0.5f, 1.0f, 
        0.5f, 0.5f, 0.5f, 1.0f,
        0.9f, 0.9f, 0.9f, 1.0f,
        0.9f, 0.9f, 0.9f, 1.0f,
    };
    m_background = new glf::RectColor();
    m_background->setColors(&backgroundColor[0]);
    m_background->setPosition(0, 0);
    m_background->setSize(m_width, m_height);
    
    // -------------------------------------------------------------- 
    // Init grid
    // -------------------------------------------------------------- 
    m_grid = new glf::Drawable(new glf::Grid(20, 20, 10.0f, 10.0f));
    m_grid->setPosition(0.0f, 0.0f, 0.0f);
    
    if (!m_gridShader.loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    m_gridShader.getUniform("Color")->setValue(0.0f, 0.0f, 0.0f, 1.0f);

    return true;
}

void Renderer::cleanup()
{
    delete m_background;
    delete m_grass;
    delete m_grid;
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_background->render();

    glm::mat4 mat;

    // draw grid
    mat = m_camera.getProjectionModelviewMatrix() * m_grid->getTransformation();
    m_gridShader.getUniform("MVP")->setValue(mat);
    m_gridShader.enable();
    m_grid->render(1);
    m_gridShader.disable();

    // draw grass
    mat = m_camera.getProjectionModelviewMatrix() * m_grass->getTransformation();
    m_currentGrassShader->getUniform("MVP")->setValue(mat);
    m_currentGrassShader->enable();
    m_grass->render(1);

    m_currentGrassShader->disable();
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
