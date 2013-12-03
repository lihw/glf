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
    GLuint numVertices = ((vres + 1) * 2 + (hres + 1) * 2);
    GLfloat *vertices = new GLfloat [numVertices * 3];

    GLfloat cellWidth = w / (GLfloat)hres;
    GLfloat cellHeight = h / (GLfloat)vres;

    GLfloat x = w * -0.5f;
    GLfloat y = h * -0.5f;

    GLfloat* v = vertices; 

    // Horizontal lines
    GLuint numIndices = (vres + 1) * 2 + (hres + 1) * 2;
    GLuint* indices = new GLuint [numIndices];
    GLuint* id = indices;
    for (GLuint i = 0; i <= vres; ++i)
    {
        v[0] = w * -0.5f;
        v[1] = 0;
        v[2] = y;
        *id++ = i * 2;
        v += 3;

        v[0] = w * 0.5f;
        v[1] = 0;
        v[2] = y;
        *id++ = i * 2 + 1;
        v += 3;

        y += cellHeight;
    }

    // Vertical lines
    for (GLuint j = 0; j <= hres; ++j)
    {
        v[0] = x;
        v[1] = 0;
        v[2] = h * -0.5f;
        *id++ = (vres + 1) * 2 + j * 2;
        v += 3;

        v[0] = x;
        v[1] = 0;
        v[2] = h * 0.5f;
        *id++ = (vres + 1) * 2 + j * 2 + 1;
        v += 3;

        x += cellWidth;
    }

    VertexAttribDescriptor desc[] =
    {
        {VERTEX_ATTRIB_POSITION, GL_FLOAT, 3}, 
    };

    _primitive = GL_LINES;
    bool ret = _vertexArray.create(GL_LINES, vertices, numVertices, &desc[0], 1, indices, numIndices);
    GLF_ASSERT(ret);
    
    delete [] vertices;
    delete [] indices;
}

Grid::~Grid()
{
}

/*
//for (GLuint i = 0; i <= vres; ++i)
    //{
    //    for (GLuint j = 0; j <= hres; ++j)
    //    {
    //        GLfloat* v = &vertices[(i * (hres + 1) + j) * 8];

    //        v[0] = x;
    //        v[1] = 0;
    //        v[2] = y;

    //        v[3] = (GLfloat)j / (GLfloat)hres;
    //        v[4] = (GLfloat)i / (GLfloat)vres;

    //        v[5] = 0.0f;
    //        v[6] = 1.0f;
    //        v[7] = 0.0f;

    //        x += cellWidth;
    //    }

    //    x = w * -0.5f;
    //    y += cellHeight;
    //}

    GLuint numIndices = (hres * 4 + 2) * vres;
    GLuint* indices = new GLuint [numIndices];
    GLuint* id = indices;
    id[0] = 0;
    id[1] = hres + 1;
    id[2] = 1;
    id[3] = hres + 1 + 1;
    //for (GLuint i = 0; i < vres; ++i)
    //{
    //    for (GLuint j = 0; j < hres; ++j)
    //    {
    //        id[0] = i * (hres + 1) + j;
    //        id[1] = (i + 1) * (hres + 1) + j;
    //        id[2] = i * (hres + 1) + j + 1;
    //        id[3] = (i + 1) * (hres + 1) + j + 1;
    //        
    //        id += 4;
    //    }

    //    // Repeat the last vertex of last row
    //    *id++ = (i + 1) * (hres + 1) + hres;
    //    // Repeat the first vertex of the first row.
    //    *id++ = (i + 1) * (hres + 1);
    //}
*/
GLF_NAMESPACE_END
