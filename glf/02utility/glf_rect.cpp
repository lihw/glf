// -------------------------------------------------------------- 
// glf_rect.cpp
// Draw a 2D rect onto the screen at specified position. 
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_rect.h"

#include "../00system/glf_assert.h"

GLF_NAMESPACE_BEGIN

const char COLOR_VERTEX_SHADER[] = 
    "#version 410 core \n"
    "\n"
    "layout (location = 0) in vec3 Position;\n"
    "\n"
    "layout (location = 0) out vec4 vColor; \n"
    "\n"
    "uniform vec4 Color[4];"
    "\n"
    "out gl_PerVertex \n"
    "{\n"
    "    vec4 gl_Position;\n"
    "};\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(Position, 1); \n"
    "    vColor = Color[gl_VertexID];\n"
    "}\n";
const char COLOR_FRAGMENT_SHADER[] = 
    "#version 410 core \n"
    "\n"
    "layout (location = 0) in vec4 vColor;\n"
    "\n"
    "layout (location = 0, index = 0) out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragColor = vColor; \n"
    "}\n";

const char TEXTURE_VERTEX_SHADER[] =
    "#version 410 core\n"
    "\n"
    "layout (location = 0) in vec4 Position;\n"
    "layout (location = 2) in vec2 Texcoord;\n"
    "\n"
    "layout (location = 0) out vec2 vTexcoord;\n"
    "\n"
    "\n"
    "out gl_PerVertex\n"
    "{\n"
    "    vec4 gl_Position;\n"
    "};\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = Position;\n"
    "    vTexcoord = Texcoord;\n"
    "}\n";
const char TEXTURE_FRAGMENT_SHADER[] = 
    "#version 410 core\n"
    "\n"
    "uniform sampler2D Texture; \n"
    "\n"
    "layout(location = 0) in vec2 vTexcoord;\n"
    "\n"
    "layout(location = 0, index = 0) out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float value = texture(Texture, vTexcoord).x;\n"
    "    FragColor = vec4(value, value, value, 1.0);\n"
    "}\n";

// -------------------------------------------------------------- 
// class Rect
// -------------------------------------------------------------- 

Rect::Rect(const char* vertexShader, 
           const char* fragmentShader)
{
    bool ret;
    // -------------------------------------------------------------- 
    // Init shaders
    // -------------------------------------------------------------- 
    ret = _shader.loadFromMemory(vertexShader, fragmentShader);
    GLF_ASSERT(ret);

    // -------------------------------------------------------------- 
    // Init vertex array
    // -------------------------------------------------------------- 
    GLfloat vertices[][5] = 
    {
        {-1.0f, -1.0f, 0, 0, 0}, 
        {1.0f,  -1.0f, 0, 1, 0}, 
        {1.0f,   1.0f, 0, 1, 1}, 
        {-1.0f,  1.0f, 0, 0, 1}, 
    };

    GLuint indices[] =
    {
        0, 1, 2, 2, 3, 0
    };

    VertexAttribDescriptor desc[] =
    {
        {VERTEX_ATTRIB_POSITION, GL_FLOAT, 3}, 
        {VERTEX_ATTRIB_TEXCOORD, GL_FLOAT, 2}, 
    };

    ret = _va.create(GL_TRIANGLES, &vertices[0][0], 4, &desc[0], 2, indices, 6);
    GLF_ASSERT(ret);
    
    // -------------------------------------------------------------- 
    // Init size and position
    // -------------------------------------------------------------- 
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    _rect[0] = 0;
    _rect[1] = 0;
    _rect[2] = viewport[2];
    _rect[3] = viewport[3];
}

Rect::~Rect()
{
}

void Rect::setPosition(GLuint x, GLuint y)
{
    _rect[0] = x;
    _rect[1] = y;
}

void Rect::setSize(GLuint width, GLuint height)
{
    _rect[2] = width;
    _rect[3] = height;
}

GLuint Rect::getWidth() const
{
    return _rect[2];
}

GLuint Rect::getHeight() const
{
    return _rect[3];
}
    
Shader* Rect::getShader() const
{
    return &_shader;
}

void Rect::render()
{
    // -------------------------------------------------------------- 
    // Setup 
    // -------------------------------------------------------------- 
    GLint origViewport[4];
    glGetIntegerv(GL_VIEWPORT, origViewport);

    glViewport(_rect[0], _rect[1], _rect[2], _rect[3]);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    _shader.enable();
        
    // -------------------------------------------------------------- 
    // Render
    // -------------------------------------------------------------- 
    _va.render(1);

    // -------------------------------------------------------------- 
    // Cleanup
    // -------------------------------------------------------------- 
    glViewport(origViewport[0], origViewport[1], origViewport[2],
            origViewport[3]);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    _shader.disable();
}

// -------------------------------------------------------------- 
// class RectColor
// -------------------------------------------------------------- 
RectColor::RectColor()
    : Rect(COLOR_VERTEX_SHADER, COLOR_FRAGMENT_SHADER)
{
    GLfloat colors[][4] = 
    {
        {0.5f, 0.5f, 0.5f, 1.0f},
        {0.5f, 0.5f, 0.5f, 1.0f},
        {0.5f, 0.5f, 0.5f, 1.0f},
        {0.5f, 0.5f, 0.5f, 1.0f},
    };

    setColors(&colors[0][0]);
}

RectColor::~RectColor()
{
}

void RectColor::setColors(const GLfloat* colors)
{
    _colors[0][0] = colors[0];
    _colors[0][1] = colors[1];
    _colors[0][2] = colors[2];
    _colors[0][3] = colors[3];
    
    _colors[1][0] = colors[4];
    _colors[1][1] = colors[5];
    _colors[1][2] = colors[6];
    _colors[1][3] = colors[7];
    
    _colors[2][0] = colors[8];
    _colors[2][1] = colors[9];
    _colors[2][2] = colors[10];
    _colors[2][3] = colors[11];
    
    _colors[3][0] = colors[12];
    _colors[3][1] = colors[13];
    _colors[3][2] = colors[14];
    _colors[3][3] = colors[15];
    
    _shader.getUniform("Color[0]")->setValue(_colors[0][0], _colors[0][1], _colors[0][2], _colors[0][3]);
    _shader.getUniform("Color[1]")->setValue(_colors[1][0], _colors[1][1], _colors[1][2], _colors[1][3]);
    _shader.getUniform("Color[2]")->setValue(_colors[2][0], _colors[2][1], _colors[2][2], _colors[2][3]);
    _shader.getUniform("Color[3]")->setValue(_colors[3][0], _colors[3][1], _colors[3][2], _colors[3][3]);
}

const GLfloat* RectColor::getColors() const
{
    return &_colors[0][0];
}


// -------------------------------------------------------------- 
// class RectTexture
// -------------------------------------------------------------- 
RectTexture::RectTexture()
    : Rect(TEXTURE_VERTEX_SHADER, TEXTURE_FRAGMENT_SHADER)
{
    _texture = NULL;
}

RectTexture::~RectTexture()
{
}

Texture* RectTexture::getTexture() const
{
    return _texture;
}

void RectTexture::setTexture(Texture* texture)
{
    GLF_ASSERT(texture != NULL);
    _texture = texture;
    _shader.getUniform("Texture")->setValue((GLuint)0);
}
    
void RectTexture::render()
{
    if (_texture != NULL)
    {
        _texture->enable(0);

        Rect::render();
        
        _texture->disable();
    }
}

GLF_NAMESPACE_END
