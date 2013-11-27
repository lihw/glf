// -------------------------------------------------------------- 
// glf_quad.cpp
// The geometry of a quad
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_quad.h"

#include "../01system/glf_assert.h"

GLF_NAMESPACE_BEGIN

Quad::Quad();
{
    // -------------------------------------------------------------- 
    // Init vertex array
    // -------------------------------------------------------------- 
    GLfloat vertices[][8] = 
    {
        {-1.0f, -1.0f, 0, 0, 0, 0, 1, 0}, 
        {1.0f,  -1.0f, 0, 1, 0, 0, 1, 0}, 
        {1.0f,   1.0f, 0, 1, 1, 0, 1, 0}, 
        {-1.0f,  1.0f, 0, 0, 1, 0, 1, 0}, 
    };

    GLuint indices[] =
    {
        0, 1, 2, 2, 3, 0
    };

    VertexAttribDescriptor desc[] =
    {
        {VERTEX_ATTRIB_POSITION, GL_FLOAT, 3}, 
        {VERTEX_ATTRIB_TEXCOORD, GL_FLOAT, 2}, 
        {VERTEX_ATTRIB_NORMAL, GL_FLOAT, 3}, 
    };

    ret = _vertexArray.create(GL_TRIANGLES, &vertices[0][0], 4, &desc[0], 3, indices, 6);
    GLF_ASSERT(ret);
}

Quad::~Quad()
{
}

GLF_NAMESPACE_END



