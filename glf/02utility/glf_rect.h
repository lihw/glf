// -------------------------------------------------------------- 
// glf_rect.h
// Draw a 2D rect onto the screen at specified position. 
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_RECT_H
#define GLF_RECT_H

#include "../glf_common.h"

#include "../01api/glf_texture.h"
#include "../01api/glf_vertex_array.h"
#include "../01api/glf_shader.h"

GLF_NAMESPACE_BEGIN

// -------------------------------------------------------------- 
// The rectangle
// -------------------------------------------------------------- 
class Rect
{
public:
    Rect(const char* vertexShader,
         const char* fragmentShader);
    virtual ~Rect();

    // Set the position.
    void setPosition(GLuint x, GLuint y);
    // The size of the rect.
    void setSize(GLuint width, GLuint height);
    // Get the size of the rect.
    GLuint getWidth() const;
    GLuint getHeight() const;
    // Get the shader of the rect.
    Shader* getShader() const;

    // Draw the rect onto screen with its lower left corner at (x, y)
    virtual void render();

protected:
    GLuint         _rect[4];
    mutable Shader _shader;
    VertexArray    _va;
};

// -------------------------------------------------------------- 
// A rectangle that need to specify 4 corner colors
// -------------------------------------------------------------- 
class RectColor : public Rect
{
public:
    RectColor();
    virtual ~RectColor();
    
    // Color of the four corners of the rect (lower left, lower right, top right, top left)
    void setColors(const GLfloat* colors);
    const GLfloat* getColors() const;

protected:
    GLfloat  _colors[4][4];
};

// -------------------------------------------------------------- 
// A rectangle in texture 
// -------------------------------------------------------------- 
class RectTexture : public Rect
{
public:
    RectTexture();
    virtual ~RectTexture();
    
    // The texture of the rect.
    void setTexture(Texture* texture);
    Texture* getTexture() const;
    
    virtual void render();


protected:
    Texture*    _texture;
};

GLF_NAMESPACE_END

#endif // !GLF_RECT_H
