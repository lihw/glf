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
    m_grid       = NULL;
    m_grass      = NULL;

    m_geometrySetting.bladeWidth               = 0.01f;
    m_geometrySetting.bladeThicknessThreshold  = 5.0f;

    m_renderingSetting.showGeometryOnly = true;
    m_renderingSetting.showNormal       = false;
    m_renderingSetting.lightingMode     = 0;
    m_renderingSetting.useTexture       = false;
    m_renderingSetting.bladeTexture     = NULL;

    m_renderingSetting.pointLight.ambient  = glm::vec4(0.5f, 0.5f, 0.5f, 1);
    m_renderingSetting.pointLight.diffuse  = glm::vec4(0.5f, 0.5f, 0.5f, 1);
    m_renderingSetting.pointLight.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1);
    m_renderingSetting.pointLight.shininess = 5.0f;
    m_renderingSetting.pointLight.position = glm::vec3(8.0f, -5.0f, 6.0f);
}

Renderer::~Renderer()
{
}

bool Renderer::initialize()
{
    glClearColor(1, 0, 0, 0);

    glDisable(GL_CULL_FACE);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    m_camera.lookAt(0, 0, 10.0f,
                    0, 0, 0,
                    0, 1, 0);

    // -------------------------------------------------------------- 
    // Initialize shaders
    // -------------------------------------------------------------- 
    if (!loadShaders())
    {
        return false;
    }

    // -------------------------------------------------------------- 
    // Load/create the grass asset
    // -------------------------------------------------------------- 
    GrassAsset* grassAsset = new GrassAsset(4.0f);
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
    
    // -------------------------------------------------------------- 
    // Init texture
    // -------------------------------------------------------------- 
    glf::Image grassBladeImage;
#define PATH_PREFIX "../demos/grass/data"
    if (!grassBladeImage.createFromFile(PATH_PREFIX"/images/blade.ppm"))
    {
        return false;
    }
#undef PATH_PREFIX 
    grassBladeImage.flipVertical();
    m_renderingSetting.bladeTexture = new glf::Texture();
    if (!m_renderingSetting.bladeTexture->create(grassBladeImage.getData(), 
                                                 grassBladeImage.getWidth(),
                                                 grassBladeImage.getHeight(), 
                                                 GL_RGB8))
    {
        GLF_LOGERROR("Failed to create blade texture");
        return false;
    }

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
    // -------------------------------------------------------------- 
    // Draw background
    // -------------------------------------------------------------- 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_background->render();

    glm::mat4 mat;

    // -------------------------------------------------------------- 
    // Draw grid
    // -------------------------------------------------------------- 
    mat = m_camera.getProjectionModelviewMatrix() * m_grid->getTransformation();
    m_shaders[GRID].getUniform("MVP")->setValue(mat);
    m_shaders[GRID].enable();
    m_grid->render(1);
    m_shaders[GRID].disable();

    mat = m_camera.getProjectionModelviewMatrix() * m_grass->getTransformation();

    if (m_renderingSetting.showGeometryOnly)
    {
        m_shaders[STEM].getUniform("MVP")->setValue(mat);
        m_shaders[STEM].enable();
        m_grass->render(1);
        m_shaders[STEM].disable();

        if (m_renderingSetting.showNormal)
        {
            m_shaders[NORMAL].getUniform("MVP")->setValue(mat);
            m_shaders[NORMAL].enable();
            m_grass->render(1);
            m_shaders[NORMAL].disable();
        }
    }
    else 
    {
        if (m_renderingSetting.useTexture)
        {
            if (m_renderingSetting.lightingMode == 0)
            {
                m_shaders[TEXTURE_ONLY].getUniform("MVP")->setValue(mat);
                m_shaders[TEXTURE_ONLY].getUniform("BladeWidth")->setValue(m_geometrySetting.bladeWidth);
                m_shaders[TEXTURE_ONLY].getUniform("ThicknessThreshold")->setValue(m_geometrySetting.bladeThicknessThreshold);
                glm::vec3 cameraPosition = m_camera.getCameraPosition();
                m_shaders[TEXTURE_ONLY].getUniform("CameraPosition")->setValue(cameraPosition.x, 
                        cameraPosition.y, cameraPosition.z); 

                m_renderingSetting.bladeTexture->enable(0);
                m_shaders[TEXTURE_ONLY].enable();
                m_grass->render(1);
                m_shaders[TEXTURE_ONLY].disable();
                m_renderingSetting.bladeTexture->disable();
            }
            else if (m_renderingSetting.lightingMode == PHONG)
            {
                m_shaders[PHONG_TEXTURE].getUniform("BladeWidth")->setValue(m_geometrySetting.bladeWidth);
                m_shaders[PHONG_TEXTURE].getUniform("ThicknessThreshold")->setValue(m_geometrySetting.bladeThicknessThreshold);
                m_shaders[PHONG_TEXTURE].getUniform("CameraPosition")->setValue(m_camera.getCameraPosition());
                m_shaders[PHONG_TEXTURE].getUniform("NormalMatrix")->setValue(glm::inverseTranspose(m_grass->getTransformation()));
                m_shaders[PHONG_TEXTURE].getUniform("MVP")->setValue(mat);
        
                m_shaders[PHONG_TEXTURE].getUniform("PointLight.position")->setValue(m_renderingSetting.pointLight.position);
                m_shaders[PHONG_TEXTURE].getUniform("PointLight.ambient")->setValue(m_renderingSetting.pointLight.ambient);
                m_shaders[PHONG_TEXTURE].getUniform("PointLight.diffuse")->setValue(m_renderingSetting.pointLight.diffuse);
                m_shaders[PHONG_TEXTURE].getUniform("PointLight.specular")->setValue(m_renderingSetting.pointLight.specular);
                m_shaders[PHONG_TEXTURE].getUniform("PointLight.shinness")->setValue(m_renderingSetting.pointLight.shininess);
                
                m_renderingSetting.bladeTexture->enable(0);
                m_shaders[PHONG_TEXTURE].enable();
                m_grass->render(1);
                m_shaders[PHONG_TEXTURE].disable();
                m_renderingSetting.bladeTexture->disable();
            }
            else
            {
                GLF_ASSERT("Invalid lighting model");
            }
        }
        else
        {
            if (m_renderingSetting.lightingMode == PHONG)
            {
                m_shaders[PHONG].getUniform("BladeWidth")->setValue(m_geometrySetting.bladeWidth);
                m_shaders[PHONG].getUniform("ThicknessThreshold")->setValue(m_geometrySetting.bladeThicknessThreshold);
                m_shaders[PHONG].getUniform("CameraPosition")->setValue(m_camera.getCameraPosition());
                m_shaders[PHONG].getUniform("NormalMatrix")->setValue(glm::inverseTranspose(m_grass->getTransformation()));
                m_shaders[PHONG].getUniform("MVP")->setValue(mat);
        
                m_shaders[PHONG].getUniform("PointLight.position")->setValue(m_renderingSetting.pointLight.position);
                m_shaders[PHONG].getUniform("PointLight.ambient")->setValue(m_renderingSetting.pointLight.ambient);
                m_shaders[PHONG].getUniform("PointLight.diffuse")->setValue(m_renderingSetting.pointLight.diffuse);
                m_shaders[PHONG].getUniform("PointLight.specular")->setValue(m_renderingSetting.pointLight.specular);
                m_shaders[PHONG].getUniform("PointLight.shinness")->setValue(m_renderingSetting.pointLight.shininess);
                
                m_shaders[PHONG].enable();
                m_grass->render(1);
                m_shaders[PHONG].disable();
            }
        }
    }
}

void Renderer::onMouseButtonDown(int x, int y, int buttons, int modifiers)
{
    if (buttons == MOUSE_BUTTON1)
    {
        m_arcball.restart();
    }
}
    
void Renderer::setBladeHeight(GLfloat height)
{
    GrassAsset* grassAsset = new GrassAsset(height);
    m_grass->getGeometry()->updateVertices(grassAsset->getVertices(),
                                           grassAsset->getNumVertices(),
                                           grassAsset->getVertexDesc(),
                                           grassAsset->getNumVertexDescEntries());
    delete grassAsset;
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

bool Renderer::loadShaders()
{
#define PATH_PREFIX "../demos/grass/data/shader"
    
    // Grid
    if (!m_shaders[GRID].loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading grid shader succeeded");
        m_shaders[GRID].getUniform("Color")->setValue(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Stem
    if (!m_shaders[STEM].loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading stem shader succeeded");
        m_shaders[STEM].getUniform("Color")->setValue(1.0f, 1.0f, 0.0f, 1.0f);
    }

    // Normal
    if (!m_shaders[NORMAL].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/grass_color.fs", NULL, NULL, 
        PATH_PREFIX"/grass_normal.gs"))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading normal shader succeeded");
        m_shaders[NORMAL].getUniform("Color")->setValue(0.0f, 0.0f, 1.0f, 1.0f);
    }

    // Texture only
    if (!m_shaders[TEXTURE_ONLY].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/grass_texture.fs", NULL, NULL,
        PATH_PREFIX"/grass.gs"))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading texture shader succeeded");
        m_shaders[TEXTURE_ONLY].getUniform("Texture")->setValue(GLuint(0));
    }
    
    // Phong
    if (!m_shaders[PHONG].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/phong.fs", NULL, NULL,
        PATH_PREFIX"/grass.gs"))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading phong shader succeeded");
    }
    
    // Phong
    if (!m_shaders[PHONG_TEXTURE].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/phong_texture.fs", NULL, NULL,
        PATH_PREFIX"/grass.gs"))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading phong+texture shader succeeded");
        m_shaders[PHONG_TEXTURE].getUniform("Texture")->setValue(GLuint(0));
    }

#undef PATH_PREFIX

    return true;
}

