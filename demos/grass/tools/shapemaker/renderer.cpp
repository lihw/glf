// renderer.cpp
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "renderer.h"

#include "grassblade.h"
#include "control.h"

Renderer::Renderer()
    : GLFAbstractRenderer()
{
    m_background            = NULL;
    m_grid                  = NULL;

    m_showNormal            = false;
    m_showGrid              = false;
    m_currentBladeIndex     = 0;
    m_currentBladeKnotIndex = 1;
    m_mode                  = EDIT_MODE;
    m_rotation              = 0;
    m_selfRotating          = false;

    m_editBlade             = NULL;
}

Renderer::~Renderer()
{
}

bool Renderer::initialize()
{
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    m_editCamera.lookAt(0, 0.0f, 10.0f,
                        0, 0.0f, 0,
                        0, 1.0f, 0.0f);
    
    m_viewCamera.lookAt(0, 5, 10.0f,
                        0, 0, 0,
                        0, 1, 0);
    
    m_viewSelfRotatingCamera.lookAt(0, 5, 10.0f,
                                    0, 0, 0,
                                    0, 1, 0);

    glPointSize(4.0f);
    glLineWidth(2.0f);

#define PATH_PREFIX "../demos/grass/data"

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
    GLfloat aspect = (GLfloat)m_width / (GLfloat)m_height;
    m_grid = new glf::Drawable(new glf::Grid(20, 20, 10.0f * aspect, 10.0f));
    m_grid->setPosition(0.0f, 0.0f, 0.0f);
    m_grid->setRotation(glf::PI * 0.5f, 0.0f, 0.0f);
    if (!m_gridShader.loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    m_gridShader.getUniform("Color")->setValue(0.0f, 0.0f, 0.0f, 1.0f);
    

    // -------------------------------------------------------------- 
    // Init grass blades
    // -------------------------------------------------------------- 
    for (GLuint i = 0; i < NUM_BLADES; ++i)
    {
        m_grassBlades[i] = new GrassBlade;
    }

    if (!m_grassBladeEditShader.loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    m_grassBladeEditShader.getUniform("Color")->setValue(0.0f, 1.0f, 0.0f, 1.0f);
    
    if (!m_grassBladeKnotShader.loadFromFiles(PATH_PREFIX"/shader/blade.vs", 
                                              PATH_PREFIX"/shader/blade_color.fs", 
                                              NULL, 
                                              NULL, 
                                              PATH_PREFIX"/shader/blade_point.gs"))
    {
        return false;
    }
    m_grassBladeKnotShader.getUniform("HighlightColor")->setValue(1.0f, 0.0f, 0.0f, 1.0f);
    m_grassBladeKnotShader.getUniform("OtherColor")->setValue(0.0f, 0.0f, 1.0f, 1.0f);
    m_grassBladeKnotShader.getUniform("CurrentVertexId")->setValue((GLuint)m_currentBladeKnotIndex);
    
    if (!m_grassBladeNormalShader.loadFromFiles(PATH_PREFIX"/shader/grass.vs", 
                                                PATH_PREFIX"/shader/grass_color.fs", 
                                                NULL, 
                                                NULL,
                                                PATH_PREFIX"/shader/grass_normal.gs"))
    {
        return false;
    }
    m_grassBladeNormalShader.getUniform("Color")->setValue(0.0f, 0.0f, 1.0f, 1.0f);
    
    if (!m_grassBladeViewShader.loadFromFiles(PATH_PREFIX"/shader/grass.vs", 
                                              PATH_PREFIX"/shader/blade_texture.fs", 
                                              NULL, 
                                              NULL, 
                                              PATH_PREFIX"/shader/grass.gs"))
    {
        return false;
    }
    m_grassBladeViewShader.getUniform("BladeWidth")->setValue(0.01f);
    //m_grassBladeViewShader.getUniform("ThicknessThreshold")->setValue(5.0f);
    m_grassBladeViewShader.getUniform("Texture")->setValue(GLuint(0));

    glf::Image grassBladeImage;
    if (!grassBladeImage.createFromFile(PATH_PREFIX"/images/blade.ppm"))
    {
        return false;
    }
    grassBladeImage.flipVertical();
    m_grassBladeTexture = new glf::Texture();
    if (!m_grassBladeTexture->create(grassBladeImage.getData(), 
                                     grassBladeImage.getWidth(),
                                     grassBladeImage.getHeight(), 
                                     GL_RGB8))
    {
        GLF_LOGERROR("Failed to create blade texture");
        return false;
    }

#undef PATH_PREFIX
    
    return true;
}

void Renderer::cleanup()
{
    delete m_background;
    delete m_grid;
    for (GLuint i = 0; i < NUM_BLADES; ++i)
    {
        delete m_grassBlades[i];
    }
}

void Renderer::render()
{
    glm::mat4 mat;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_background->render();

    if (m_mode == EDIT_MODE)
    {
        // draw grid
        if (m_showGrid)
        {
            mat = m_editCamera.getProjectionModelviewMatrix() * m_grid->getTransformation();
            m_gridShader.getUniform("MVP")->setValue(mat);
            m_gridShader.enable();
            m_grid->render(1);
            m_gridShader.disable();
        }

        // draw grass blade
        mat = m_editCamera.getProjectionModelviewMatrix();
        m_grassBladeEditShader.getUniform("MVP")->setValue(mat);
        m_grassBladeEditShader.enable();
        m_grassBlades[m_currentBladeIndex]->render(1);
        m_grassBladeEditShader.disable();

        m_grassBladeKnotShader.getUniform("MVP")->setValue(mat);
        m_grassBladeKnotShader.enable();
        m_grassBlades[m_currentBladeIndex]->render(1);
        m_grassBladeKnotShader.disable();

        if (m_showNormal)
        {
            mat = m_editCamera.getProjectionModelviewMatrix();
            m_grassBladeNormalShader.getUniform("MVP")->setValue(mat);
            m_grassBladeNormalShader.enable();
            m_grassBlades[m_currentBladeIndex]->render(1);
            m_grassBladeNormalShader.disable();
        }
    }
    else // view mode
    {
        glm::vec3 cameraPosition = m_viewCamera.getCameraPosition();
        m_grassBladeViewShader.getUniform("CameraPosition")->setValue(cameraPosition.x, cameraPosition.y, cameraPosition.z); 

        if (m_selfRotating)
        {
            mat = m_viewSelfRotatingCamera.getProjectionModelviewMatrix() * glm::eulerAngleY(m_rotation);;
            m_rotation += 0.01f;
        }
        else
        {
            mat = m_viewCamera.getProjectionModelviewMatrix();
        }
            
        m_grassBladeViewShader.getUniform("MVP")->setValue(mat);

        m_grassBladeTexture->enable(0);
        m_grassBladeViewShader.enable();
        m_grassBlades[m_currentBladeIndex]->render(1);
        m_grassBladeViewShader.disable();
        m_grassBladeTexture->disable();

    }
}

void Renderer::onMouseButtonDown(int x, int y, int buttons, int modifiers)
{
    if (m_mode == VIEW_MODE)
    {
        if (buttons == MOUSE_BUTTON1)
        {
            m_arcball.restart();
        }
    }
    else if (m_mode == EDIT_MODE)
    {
        m_editBlade = m_grassBlades[m_currentBladeIndex];
    }
}

void Renderer::onMouseButtonUp(int x, int y, int buttons, int modifiers)
{
    if (m_mode == EDIT_MODE)
    {
        m_editBlade = NULL;
    }
}

void Renderer::onMouseWheel(int numDegrees)
{
    if (m_mode == VIEW_MODE)
    {
        m_viewCamera.translate(0, 0, (GLfloat)numDegrees * 0.1f);
    }
    
}

void Renderer::onMouseMove(int x, int y, int buttons, int modifiers)
{
    if (m_mode == VIEW_MODE)
    {
        if (buttons == MOUSE_BUTTON1)
        {
            GLfloat cx, cy;
            cx = (GLfloat)x / (GLfloat)m_width;
            cy = ((GLfloat)m_height - 1.0f - (GLfloat)y) / (GLfloat)m_height;

            m_arcball.updateMouse(cx, cy);
            m_viewCamera.setRotation(&(m_arcball.getRotationMatrix()[0][0]));

        }       
    }
    else if (m_mode == EDIT_MODE && m_editBlade != NULL)
    {
        glm::vec3 p = m_editBlade->getKnotPosition(m_currentBladeKnotIndex);
        
        GLfloat nx = (GLfloat)x / (GLfloat)m_width;
        GLfloat ny = (GLfloat)(m_height - 1 - y) / (GLfloat)m_height;

        p.x = 6.0f * nx - 3.0f;
        p.y = 6.0f * ny - 1.0f;

        m_editBlade->setKnotPosition(m_currentBladeKnotIndex, p);

        Control* ctrl = (Control*)glfGetControl();
        ctrl->syncKnotPosition(p);
    }

}
    
void Renderer::onKeyUp(int key, int modifiers)
{
    if (key == Qt::Key_Escape)
    {
        glfGetApplication()->quit();
    }
}

void Renderer::onResized(int w, int h)
{
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    m_editCamera.setOrthogonal(-3.0f * aspect, 3.0f * aspect, -1.0f, 5.0f, 0.0f, 100.0f);
    m_viewCamera.setOrthogonal(-3.0f * aspect, 3.0f * aspect, -1.0f, 5.0f, 0.0f, 100.0f);
    m_viewSelfRotatingCamera.setOrthogonal(-2.5f * aspect, 2.5f * aspect, -1.0f, 5.0f, 0.0f, 100.0f);
    glViewport(0, 0, w, h);

    m_width = w;
    m_height = h;
}

