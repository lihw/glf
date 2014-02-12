// renderer.cpp
//
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "renderer.h"

#include "grass_asset.h"

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
    m_rotatingLight = false;

    m_background = NULL;
    m_grid       = NULL;
    m_grass      = NULL;
    m_light      = NULL;

    m_geometrySetting.bladeHeight              = 2.5f;
    m_geometrySetting.bladeWidth               = 0.06f;
    m_geometrySetting.bladeThicknessThreshold  = 2.0f;

    m_renderingSetting.showGeometryOnly = false;
    m_renderingSetting.showNormal       = false;
    m_renderingSetting.lightingMode     = PHONG;
    m_renderingSetting.useTexture       = true;
    m_renderingSetting.bladeTexture     = NULL;

    m_renderingSetting.light.ambient  = glm::vec4(0.0f, 0.0f, 0.0f, 1);
    m_renderingSetting.light.diffuse  = glm::vec4(0.5f, 0.5f, 0.5f, 1);
    m_renderingSetting.light.specular = glm::vec4(0.0f, 0.0f, 0.0f, 1);
    m_renderingSetting.light.shininess = 5.0f;
    m_renderingSetting.light.position = glm::vec3(-1.0f, 0.0f, 0.0f);

    m_renderingSetting.smFramebuffer = NULL;
    m_renderingSetting.smShow        = false;
    m_renderingSetting.smSize        = 512;

    m_renderingSetting.bladeTexture  = NULL;

    m_renderingSetting.ambientOcclusion = true;
    m_splatTextureIndexX = NULL;
    m_splatTextureIndexY = NULL;
    m_splatTextureX1 = NULL;
    m_splatTextureY1 = NULL;
    m_splatTextureX2 = NULL;
    m_splatTextureY2 = NULL;
    m_aoAtomic = NULL;
    m_aoInitialIndex = NULL;      
    m_aoFramebuffer = NULL;
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
    // Load/create the grass asset
    // -------------------------------------------------------------- 
    GrassAsset* grassAsset = new GrassAsset(m_geometrySetting.bladeHeight);
    glf::Mesh* grassMesh = new glf::Mesh(grassAsset->getVertices(),
                                         grassAsset->getNumVertices(),
                                         grassAsset->getIndices(),
                                         grassAsset->getNumIndices(),
                                         grassAsset->getPrimitive(),
                                         grassAsset->getVertexDesc(),
                                         grassAsset->getNumVertexDescEntries());
    m_grass = new glf::Drawable(grassMesh);
    m_grassBox = grassAsset->getBoundingBox();
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
    // Init the light
    // -------------------------------------------------------------- 
    m_light= new glf::Drawable(new glf::LineStrip());
    m_light->setScaling(4.0f, 1.0f, 1.0f);
    m_light->setPosition(0.0f, 0.0f, 0.0f);
    
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

    // -------------------------------------------------------------- 
    // Init ambient occlusion
    // -------------------------------------------------------------- 
    m_splatTexture = new glf::ImageStorage();
    m_startOffsetTexture = new glf::ImageStorage();
    if (!m_splatTexture->create2D(GL_RGBA32UI, m_setting.aoSize * 2, m_setting.aoSize * m_setting.aoNumLayers))
    {
        GLF_LOGERROR("Failed to create splat textures");
        return false;
    }
    m_startOffsetTexture = new glf::ImageStorage();
    if (!m_startOffsetTexture->create2D(GL_R32UI, m_setting.aoSize, m_setting.aoSize))
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
    
    m_aoRect = new glf::Rect(AOCLEAR_VS, AOCLEAR_FS);
    m_aoRect->setSize(m_setting.aoSize, m_setting.aoSize);

    m_aoCameras[0].lookAt(0.0f, m_scene->getAABB().max().y + 0.01, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    m_aoCameras[0].setOrthogonal(m_scene->getAABB().min().x - 0.01f, 
                                 m_scene->getAABB().max().x + 0.01f,
                                 m_scene->getAABB().min().z - 0.01f,
                                 m_scene->getAABB().max().z + 0.01f,
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

    // -------------------------------------------------------------- 
    // Create shadow map framebuffer
    // -------------------------------------------------------------- 
    colorFormats[0] = GL_RGBA32F;
    m_renderingSetting.smFramebuffer = new glf::Framebuffer();
    ret = m_renderingSetting.smFramebuffer->create(m_renderingSetting.smSize,
                                                   m_renderingSetting.smSize,
                                                   colorFormats,
                                                   1,
                                                   GL_DEPTH_COMPONENT24,
                                                   0);
    if (!ret)
    {
        GLF_LOGERROR("Failed to create the framebuffer for shadow map");
        return false;
    }
        
    // Visualize the shadow map
    m_shadowmap = new glf::RectTexture();
    m_shadowmap->setTexture(m_renderingSetting.smFramebuffer->getColorBuffer(0));
    m_shadowmap->setSize(512, 512);
    m_shadowmap->setPosition(0, 0);
    
    
    // Visualize the ambient occlusion map
    m_ao = new glf::RectTexture();
    m_ao->setTexture(m_aoFramebuffer->getColorBuffer(0));
    m_ao->setSize(256, 256);
    m_ao->setPosition(512, 0);
    
    return true;
}

void Renderer::cleanup()
{
    delete m_background;
    delete m_grass;
    delete m_grid;
    delete m_shadowmap;
    delete m_light;

    delete m_renderingSetting.smFramebuffer;
    delete m_renderingSetting.bladeTexture;
    delete m_splatTextureX1;
    delete m_splatTextureY1;
    delete m_splatTextureX2;
    delete m_splatTextureY2;
    delete m_splatTextureIndexX;
    delete m_splatTextureIndexY;
    delete m_aoAtomic;
    delete m_aoRect;
    delete [] m_aoInitialIndex;
    delete m_aoFramebuffer;
    delete m_ao;
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

    // -------------------------------------------------------------- 
    // Render shadow map
    // -------------------------------------------------------------- 
    if (m_renderingSetting.translucency || m_renderingSetting.smShow)
    {
        renderShadowMap();
    }

    // -------------------------------------------------------------- 
    // Render ambient occlusion if needed
    // -------------------------------------------------------------- 
    if (m_renderingSetting.ambientOcclusion)
    {
        generateAmbientOcclusion();
    }

    // -------------------------------------------------------------- 
    // Draw grass
    // -------------------------------------------------------------- 
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
                m_shaders[TEXTURE_ONLY].getUniform("ScreenSize")->setValue((GLfloat)m_width, (GLfloat)m_height);
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
                glf::Shader* shader = NULL;

                if (m_renderingSetting.ambientOcclusion)
                {
                    shader = &m_shaders[AO];
                }
                else
                {
                    if (m_renderingSetting.translucency)
                    {
                        shader = &m_shaders[PHONG_TEXTURE_SSS];

                        glf::Camera camera;
                        camera.fromLight(m_renderingSetting.light, m_grassBox);
                        glm::mat4 mat = camera.getProjectionModelviewMatrix() * m_grass->getTransformation();

                        shader->getUniform("LightMVP")->setValue(mat);
                        shader->getUniform("Scale")->setValue(2.0f);
                    }
                    else
                    {
                        shader = &m_shaders[PHONG_TEXTURE];
                    }
                }

                if (shader != &m_shaders[AO])
                {
                    shader->getUniform("BladeWidth")->setValue(m_geometrySetting.bladeWidth);
                    shader->getUniform("NormalMatrix")->setValue(glm::inverseTranspose(m_grass->getTransformation()));
                    shader->getUniform("MVP")->setValue(mat);
            
                    shader->getUniform("Light.direction")->setValue(m_renderingSetting.light.position);
                    shader->getUniform("Light.ambient")->setValue(m_renderingSetting.light.ambient);
                    shader->getUniform("Light.diffuse")->setValue(m_renderingSetting.light.diffuse);
                    shader->getUniform("Light.specular")->setValue(m_renderingSetting.light.specular);
                    shader->getUniform("Light.shinness")->setValue(m_renderingSetting.light.shininess);
                        
                    m_renderingSetting.bladeTexture->enable(0);
                    m_renderingSetting.smFramebuffer->getColorBuffer(0)->enable(1);
                    shader->enable();

                    m_grass->render(1);

                    shader->disable();
                    m_renderingSetting.smFramebuffer->getColorBuffer(0)->disable();
                    m_renderingSetting.bladeTexture->disable();
                }
                else
                {
                    //mat = m_aoCameras[0].getProjectionModelviewMatrix() * m_grass->getTransformation();
                    shader->getUniform("BladeWidth")->setValue(m_geometrySetting.bladeWidth);
                    shader->getUniform("MVP")->setValue(mat);

                    shader->getUniform("MVPX")->setValue(m_aoCameras[0].getProjectionModelviewMatrix());
                    //shader->getUniform("MVPY")->setValue(m_aoCameras[1].getProjectionModelviewMatrix());

                    m_splatTextureIndexX->enableRead(0);
                    m_splatTextureX1->enableRead(1);
                    m_splatTextureX2->enableRead(2);
                    
                    m_splatTextureIndexY->enableRead(3);
                    m_splatTextureY1->enableRead(4);
                    m_splatTextureY2->enableRead(5);

                    m_aoFramebuffer->getColorBuffer(0)->enable(6);

                    shader->enable();
                    m_grass->render(1);
                    shader->disable();

                    m_splatTextureIndexX->disable();
                    m_splatTextureX1->disable();
                    m_splatTextureX2->disable();
                    
                    m_aoFramebuffer->getColorBuffer(0)->disable();
                    m_splatTextureIndexY->disable();
                    m_splatTextureY1->disable();
                    m_splatTextureY2->disable();
                }
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
                m_shaders[PHONG].getUniform("NormalMatrix")->setValue(glm::inverseTranspose(m_grass->getTransformation()));
                m_shaders[PHONG].getUniform("MVP")->setValue(mat);
        
                m_shaders[PHONG].getUniform("Light.direction")->setValue(m_renderingSetting.light.position);
                m_shaders[PHONG].getUniform("Light.ambient")->setValue(m_renderingSetting.light.ambient);
                m_shaders[PHONG].getUniform("Light.diffuse")->setValue(m_renderingSetting.light.diffuse);
                m_shaders[PHONG].getUniform("Light.specular")->setValue(m_renderingSetting.light.specular);
                m_shaders[PHONG].getUniform("Light.shinness")->setValue(m_renderingSetting.light.shininess);
                
                m_shaders[PHONG].enable();
                m_grass->render(1);
                m_shaders[PHONG].disable();
            }
        }
    }

    if (m_rotatingLight)
    {
        mat = m_camera.getProjectionModelviewMatrix() * glm::mat4(m_arcballLight.getRotationMatrix()) * m_light->getTransformation();
        m_shaders[STEM].getUniform("MVP")->setValue(mat);
        m_shaders[STEM].enable();
        m_light->render(1);
        m_shaders[STEM].disable();
    }

    if (m_renderingSetting.smShow)
    {
        m_shadowmap->render();
    }

    if (m_renderingSetting.ambientOcclusion)
    {
        m_ao->render();
    }

}

void Renderer::onMouseButtonDown(int x, int y, int buttons, int modifiers)
{
    if (buttons == MOUSE_BUTTON1)
    {
        if (modifiers & KEYBOARD_CTRL)
        {
            m_rotatingLight = true;
            m_arcballLight.restart();
        }
        else
        {
            m_rotatingLight = false;
            m_arcball.restart();
        }
    }
}
    
void Renderer::setBladeHeight(GLfloat height)
{
    m_geometrySetting.bladeHeight = height;
    GrassAsset* grassAsset = new GrassAsset(height);
    m_grass->getGeometry()->updateVertices(grassAsset->getVertices(),
                                           grassAsset->getNumVertices(),
                                           grassAsset->getVertexDesc(),
                                           grassAsset->getNumVertexDescEntries());
    m_grassBox = grassAsset->getBoundingBox();
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
        
        if (!m_rotatingLight)
        {
            m_arcball.updateMouse(cx, cy);
            m_camera.setRotation(m_arcball.getRotationMatrixF());
        }
        else
        {
            m_arcballLight.updateMouse(cx, cy);
            glm::mat3 mat = m_arcballLight.getRotationMatrix();
            m_renderingSetting.light.position = glm::inverseTranspose(mat) * glm::vec3(-1, 0, 0);
        }
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
    
    // Phong + texture
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
    
    // Shadow map generation
    if (!m_shaders[SHADOWMAP].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/shadowmap.fs", NULL, NULL,
        PATH_PREFIX"/grass_expansion.gs"))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading shadow map shader succeeded");
    }

    // Phong + texture + scattering
    if (!m_shaders[PHONG_TEXTURE_SSS].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/phong_texture_sss.fs", NULL, NULL,
        PATH_PREFIX"/grass.gs"))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading phong+texture+subsurface scattering shader succeeded");
        m_shaders[PHONG_TEXTURE_SSS].getUniform("Texture")->setValue(GLuint(0));
        m_shaders[PHONG_TEXTURE_SSS].getUniform("ShadowMap")->setValue(GLuint(1));
    }

    // AO
    if (!m_shaders[AO_GENERATE].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/ao_gen.fs", NULL, NULL,
        PATH_PREFIX"/grass.gs"))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading ambient occlusion generation shader succeeded");
    }
    if (!m_shaders[AO].loadFromFiles(PATH_PREFIX"/grass.vs", PATH_PREFIX"/ao.fs", NULL, NULL,
        PATH_PREFIX"/grass.gs"))
    {
        return false;
    }
    else
    {
        GLF_LOGINFO("Loading ambient occlusion rendering shader succeeded");
        //m_shaders[AO].getUniform("Texture")->setValue(GLuint(6));
    }

#undef PATH_PREFIX

    return true;
}

    
void Renderer::renderShadowMap()
{
    GLF_ASSERT(m_renderingSetting.smFramebuffer != NULL);

    m_renderingSetting.smFramebuffer->enable();

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);

    glf::Camera camera;
    camera.fromLight(m_renderingSetting.light, m_grassBox);
    glm::mat4 mat = camera.getProjectionModelviewMatrix() * m_grass->getTransformation();

    glViewport(0, 0, m_renderingSetting.smSize, m_renderingSetting.smSize);

    m_shaders[SHADOWMAP].getUniform("MVP")->setValue(mat);
    m_shaders[SHADOWMAP].getUniform("BladeWidth")->setValue(m_geometrySetting.bladeWidth * 1.1f); // Increase the blade width
    m_shaders[SHADOWMAP].getUniform("ThicknessThreshold")->setValue(m_geometrySetting.bladeThicknessThreshold);
    glm::vec3 cameraPosition = camera.getCameraPosition();
    m_shaders[SHADOWMAP].getUniform("CameraPosition")->setValue(cameraPosition.x, 
            cameraPosition.y, cameraPosition.z); 

    m_shaders[SHADOWMAP].enable();
    m_grass->render(1);
    m_shaders[SHADOWMAP].disable();

    m_renderingSetting.smFramebuffer->disable();
    
    glViewport(0, 0, m_width, m_height);

    glEnable(GL_CULL_FACE);
}
    
void Renderer::generateAmbientOcclusion()
{
    m_aoFramebuffer->enable();
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, GRID_XRES, GRID_YRES);
    
    m_shaders[AO_GENERATE].getUniform("BladeWidth")->setValue(m_geometrySetting.bladeWidth * 1.1f); // Increase the blade width
    //m_shaders[AO_GENERATE].getUniform("ThicknessThreshold")->setValue(m_geometrySetting.bladeThicknessThreshold);

    glf::ImageStorage *buffers[] =
    {
        m_splatTextureIndexX,
        m_splatTextureX1, 
        m_splatTextureX2,

        m_splatTextureIndexY,
        m_splatTextureY1, 
        m_splatTextureY2,
    };

    for (GLuint i = 0; i < 1; ++i)
    {
        m_aoAtomic->enable(0);
        m_aoAtomic->set(0);

        buffers[i * 3 + 0]->enableReadAndWrite(1);
        buffers[i * 3 + 1]->enableWrite(2);
        buffers[i * 3 + 2]->enableWrite(3);

        // Clear offset buffer to -1
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        m_aoRect->render(); 

        //buffers[i * 3]->uploadData(m_aoInitialIndex);

        // Write splats into grids.
        glf::Camera* camera = &m_aoCameras[i];
        //glf::Camera* camera = &m_camera;
        
        glm::mat4 mat = camera->getProjectionModelviewMatrix() * m_grass->getTransformation();
        m_shaders[AO_GENERATE].getUniform("MVP")->setValue(mat);
        //glm::vec3 cameraPosition = camera->getCameraPosition();
        //m_shaders[AO_GENERATE].getUniform("CameraPosition")->setValue(cameraPosition.x, 
        //    cameraPosition.y, cameraPosition.z); 

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        m_shaders[AO_GENERATE].enable();
        m_grass->render(1);
        m_shaders[AO_GENERATE].disable();

        m_aoAtomic->disable();
        buffers[i * 3 + 0]->disable();
        buffers[i * 3 + 1]->disable();
        buffers[i * 3 + 2]->disable();
    }
        
    glViewport(0, 0, m_width, m_height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_aoFramebuffer->disable();
}
