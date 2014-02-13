// renderer.cpp
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "renderer.h"

#include "scene.h"

const char AOCLEAR_VS[] =
    "#version 430 core\n"
    "\n"
    "layout (location = 0) in vec4 Position;\n"
    "\n"
    "out gl_PerVertex\n"
    "{\n"
    "    vec4 gl_Position;\n"
    "};\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = Position;\n"
    "}\n";

const char AOCLEAR_FS[] = 
    "#version 430 core\n"
    "in vec4 gl_FragCoord; \n"
    "layout (location = 0, index = 0) out vec4 FragColor;\n"
    "layout(binding = 1, r32ui) coherent uniform uimage2D StartOffsetBuffer; \n"
    "void main()\n"
    "{\n"
    "    uvec4 value = uvec4(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);\n"
    "    imageStore(StartOffsetBuffer, ivec2(gl_FragCoord.xy), value); \n"
    "\n"
    "    FragColor = vec4(0, 0, 0, 0); \n"
    "}\n";

Renderer::Renderer()
    : GLFAbstractRenderer()
{
    m_background = NULL;
    m_grid       = NULL;
    m_scene      = NULL;

    m_setting.aoEnabled   = true;
    m_setting.aoSize      = 512;
    m_setting.aoNumLayers = 32;

    m_aoFragmentTexture    = NULL;
    m_aoStartOffsetTexture = NULL;
    m_aoAtomic = NULL;
    m_aoFramebuffer = NULL;
    m_aoClearRect = NULL;
    m_aoDebugRect = NULL;
    m_aoBlendRect = NULL;
}

Renderer::~Renderer()
{
}

bool Renderer::initialize()
{
    glClearColor(1, 0, 0, 0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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
    // Init scene
    // -------------------------------------------------------------- 
    m_scene = new Scene();
    
    // -------------------------------------------------------------- 
    // Init ambient occlusion textures and other stuff
    // -------------------------------------------------------------- 
    m_aoFragmentTexture = new glf::ImageStorage();
    m_aoStartOffsetTexture = new glf::ImageStorage();
    if (!m_aoFragmentTexture->create2D(GL_RGBA32UI, m_setting.aoSize * 2, m_setting.aoSize * m_setting.aoNumLayers))
    {
        GLF_LOGERROR("Failed to create splat textures");
        return false;
    }
    m_aoStartOffsetTexture = new glf::ImageStorage();
    if (!m_aoStartOffsetTexture->create2D(GL_R32UI, m_setting.aoSize, m_setting.aoSize))
    {
        GLF_LOGERROR("Failed to create splat index textures");
        return false;
    }
    
    m_aoAtomic = new glf::Atomic();
    if (!m_aoAtomic->create())
    {
        GLF_LOGERROR("Failed to create the atomic counter for ambient occlusion");
        return false;
    }
    
    m_aoClearRect = new glf::Rect(AOCLEAR_VS, AOCLEAR_FS);
    m_aoClearRect->setSize(m_setting.aoSize, m_setting.aoSize);
    
   
    m_aoCamera.lookAt(0.0f, m_scene->getAABB().max().y + 0.01, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    m_aoCamera.setOrthogonal(m_scene->getAABB().min().x - 0.05f, 
                             m_scene->getAABB().max().x + 0.05f,
                             m_scene->getAABB().min().z - 0.05f,
                             m_scene->getAABB().max().z + 0.05f,
                             0.01f,
                             m_scene->getAABB().max().y - m_scene->getAABB().min().y + 0.01f);

    bool ret;
    GLenum colorFormats[] = { GL_RGBA8 };
    m_aoFramebuffer = new glf::Framebuffer();
    ret = m_aoFramebuffer->create(m_setting.aoSize,
                                  m_setting.aoSize,
                                  colorFormats,
                                  1,
                                  GL_DEPTH_COMPONENT24,
                                  0);
    if (!ret)
    {
        GLF_LOGERROR("Failed to create the framebuffer for ambient occlusion debug");
        return false;
    }

    m_aoDebugRect = new glf::RectTexture();
    m_aoDebugRect->setTexture(m_aoFramebuffer->getColorBuffer(0));
    m_aoDebugRect->setSize(m_setting.aoSize, m_setting.aoSize);
    m_aoDebugRect->setPosition(0, 0);

#define PATH_PREFIX "../demos/vao/data/shader"
    const char* blendVS = glf::Shader::readTextFile(PATH_PREFIX"/blend.vs");
    const char* blendFS = glf::Shader::readTextFile(PATH_PREFIX"/ao_blend.fs");
    m_aoBlendRect = new glf::Rect(blendVS, blendFS);
    m_aoBlendRect->setSize(m_width, m_height);
    m_aoBlendRect->setPosition(0, 0);
    delete [] blendVS;
    delete [] blendFS;
#undef PATH_PREFIX
    
    return true;
}

void Renderer::cleanup()
{
    delete m_background;
    delete m_scene;
    delete m_grid;

    delete m_aoFragmentTexture;
    delete m_aoStartOffsetTexture;
    delete m_aoAtomic;
    delete m_aoClearRect;
    delete m_aoDebugRect;
    delete m_aoBlendRect;
    delete m_aoFramebuffer;
}

void Renderer::render()
{
    // -------------------------------------------------------------- 
    // Draw ambient occlusion map
    // -------------------------------------------------------------- 
    if (m_setting.aoEnabled)
    {
        generateAmbientOcclusionMap();
    }

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

    m_gridShader.getUniform("MVP")->setValue(mat);
    m_gridShader.enable();
    m_grid->render(1);
    m_gridShader.disable();

    // -------------------------------------------------------------- 
    // Draw scene
    // -------------------------------------------------------------- 
    mat = m_camera.getProjectionModelviewMatrix();

    m_sceneShader.getUniform("MVP")->setValue(mat);
    m_sceneShader.enable();
    m_scene->render();
    m_sceneShader.disable();

    // -------------------------------------------------------------- 
    // Render ambient occlusion map 
    // -------------------------------------------------------------- 
    if (m_setting.aoEnabled)
    {
        m_aoDebugRect->render();
    }
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
        m_camera.setRotation(m_arcball.getRotationMatrixF());
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
#define PATH_PREFIX "../demos/vao/data/shader"
    
    // Grid
    if (!m_gridShader.loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading grid shader succeeded");
        m_gridShader.getUniform("Color")->setValue(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Scene
    if (!m_sceneShader.loadFromLibrary(glf::ShaderLibrary::COLOR, glf::ShaderLibrary::COLOR))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading stem shader succeeded");
        m_sceneShader.getUniform("Color")->setValue(1.0f, 1.0f, 0.0f, 1.0f);
    }

    // AO generate shader
    if (!m_aoGenerateShader.loadFromFiles(PATH_PREFIX"/simple.vs", PATH_PREFIX"/ao_generate.fs", NULL, NULL, NULL))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading ambient occlusion map generation shader succeeded");
    }
    /*
    // AO render shader
    if (!m_aoRenderShader.loadFromFiles(PATH_PREFIX"/simple.vs", PATH_PREFIX"/ao_render.fs", NULL, NULL, NULL))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading ambient occlusion rendering shader succeeded");
    }
    */
#undef PATH_PREFIX

    return true;
}

    
void Renderer::generateAmbientOcclusionMap()
{
    // -------------------------------------------------------------- 
    // Start 
    // -------------------------------------------------------------- 
    m_aoFramebuffer->enable();
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, m_setting.aoSize, m_setting.aoSize);
    
    m_aoAtomic->enable(0);
    m_aoAtomic->set(0);

    m_aoStartOffsetTexture->enableWrite(1);
    m_aoFragmentTexture->enableReadAndWrite(2);
    
    // -------------------------------------------------------------- 
    // Render fragments into UAV buffers
    // -------------------------------------------------------------- 

    // Clear offset buffer to -1
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    m_aoClearRect->render(); 

    // Write splats into grids.
    glm::mat4 mvp = m_aoCamera.getProjectionModelviewMatrix();
    m_aoGenerateShader.getUniform("MVP")->setValue(mvp);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    m_aoGenerateShader.enable();
    m_scene->render();
    m_aoGenerateShader.disable();
    
    // -------------------------------------------------------------- 
    // Blend fragments into a texture
    // -------------------------------------------------------------- 
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_aoBlendRect->render();
    
    // -------------------------------------------------------------- 
    // Cleanup
    // -------------------------------------------------------------- 
    m_aoAtomic->disable();
    m_aoFragmentTexture->disable();
    m_aoStartOffsetTexture->disable();
        
    glViewport(0, 0, m_width, m_height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_aoFramebuffer->disable();
}
