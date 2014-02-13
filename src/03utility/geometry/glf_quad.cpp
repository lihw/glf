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

#include "../../01system/glf_assert.h"

GLF_NAMESPACE_BEGIN

Quad::Quad()
{
    // -------------------------------------------------------------- 
    // Init vertex array
    // -------------------------------------------------------------- 
    GLfloat vertices[][8] = 
    {
        {-1.0f, 0, -1.0f, 0, 0, 0, 1, 0}, 
        {1.0f,  0, -1.0f, 1, 0, 0, 1, 0}, 
        {1.0f,  0,  1.0f, 1, 1, 0, 1, 0}, 
        {-1.0f, 0,  1.0f, 0, 1, 0, 1, 0}, 
    };

    createQuad(&vertices[0][0]);
}

Quad::Quad(const GLfloat* points)
{
    GLfloat* vertices = new GLfloat [4 * 8];

    GLfloat t[] =
    {
        0, 0,
        1, 0,
        1, 1,
        0, 1
    };

    // Fill each vertex
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat* v = &vertices[i * 8];

        const GLfloat* p0 = &points[((i + 3) % 4) * 3];
        const GLfloat* p1 = &points[i * 3];
        const GLfloat* p2 = &points[(i + 1) * 3];
        glm::vec3 e0 = glm::vec3(p0[0], p0[1], p0[2]) - glm::vec3(p1[0], p1[1], p1[2]);
        glm::vec3 e1 = glm::vec3(p2[0], p2[1], p2[2]) - glm::vec3(p1[0], p1[1], p1[2]);

        glm::vec3 n = glm::normalize(glm::cross(e0, e1));

        v[0] = points[i * 3];
        v[1] = points[i * 3 + 1];
        v[2] = points[i * 3 + 2];

        v[3] = t[i * 2 + 0];
        v[4] = t[i * 2 + 1];

        v[5] = n.x;
        v[6] = n.y;
        v[7] = n.z;
    }

    createQuad(vertices);

    delete [] vertices;
}

Quad::~Quad()
{
}

void Quad::createQuad(const GLfloat* vertices)
{
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

    _primitive = GL_TRIANGLES;

    bool ret = _vertexArray.create(GL_TRIANGLES, vertices, 4, &desc[0], 3, indices, 6);
    GLF_ASSERT(ret);
}
    
void Quad::fillScreen()
{
    static GLuint vertexArray = 0;
    if (vertexArray == 0)
    {
        glGenVertexArrays(1, &vertexArray);
    }

    GLfloat vertices[][3] = 
    {
        {-1.0f, 0, -1.0f}, 
        {-1.0f,  0, 1.0f}, 
        { 1.0f,  0,  1.0f}, 
        { 1.0f, 0,  -1.0f}, 
    };

    glBindVertexArray(vertexArray);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
}

GLF_NAMESPACE_END



