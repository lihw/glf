// -------------------------------------------------------------- 
// glf_grid.cpp
// The grid
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_grid.h"

#include "../../01system/glf_assert.h"

GLF_NAMESPACE_BEGIN

Grid::Grid(GLuint hres, GLuint vres, GLfloat w, GLfloat h)
{
    GLuint numVertices = (hres + 1) * (vres + 1);
    GLfloat *vertices = new GLfloat [numVertices * 8];

    GLfloat cellWidth = w / (GLfloat)hres;
    GLfloat cellHeight = h / (GLfloat)vres;

    GLfloat x = w * -0.5f;
    GLfloat y = h * -0.5f;

    for (GLuint i = 0; i <= vres; ++i)
    {
        for (GLuint j = 0; j <= hres; ++j)
        {
            GLfloat* v = &vertices[(i * (vres + 1) + j) * 8];

            v[0] = x;
            v[1] = 0;
            v[2] = y;

            v[3] = (GLfloat)j / (GLfloat)hres;
            v[4] = (GLfloat)i / (GLfloat)vres;

            v[5] = 0.0f;
            v[6] = 1.0f;
            v[7] = 0.0f;

            x += cellWidth;
        }

        x = w * -0.5f;
        y += cellHeight;
    }

    GLuint numIndices = (hres * 4 + 2) * vres;
    GLuint* indices = new GLuint [numIndices];
    GLuint* id = indices;
    for (GLuint i = 0; i < vres; ++i)
    {
        for (GLuint j = 0; j < hres; ++j)
        {
            id[0] = i * (hres + 1) + j;
            id[1] = (i + 1) * (hres + 1) + j;
            id[2] = i * (hres + 1) + j + 1;
            id[3] = (i + 1) * (hres + 1) + j + 1;
            
            id += 4;
        }

        // Repeat the last vertex of last row
        *id++ = (i + 1) * (hres + 1) + hres + 1;
        // Repeat the first vertex of the first row.
        *id++ = (i + 1) * (hres + 1);
    }

    VertexAttribDescriptor desc[] =
    {
        {VERTEX_ATTRIB_POSITION, GL_FLOAT, 3}, 
        {VERTEX_ATTRIB_TEXCOORD, GL_FLOAT, 1}, 
        {VERTEX_ATTRIB_NORMAL, GL_FLOAT, 3}, 
    };

    _primitive = GL_LINE_STRIP;
    bool ret = _vertexArray.create(GL_TRIANGLE_STRIP, vertices, numVertices, &desc[0], 3, indices, numIndices);
    GLF_ASSERT(ret);
    
    delete [] vertices;
    delete [] indices;
}

Grid::~Grid()
{
}


GLF_NAMESPACE_END
