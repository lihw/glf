// -------------------------------------------------------------- 
// glf_linestrip.cpp
// The geometry of a line strip
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_line_strip.h"

#include "../../01system/glf_assert.h"


GLF_NAMESPACE_BEGIN

LineStrip::LineStrip(const GLfloat* points, GLuint numPoints)
{
    createLineStrip(points, numPoints);
}

LineStrip::LineStrip()
{
    GLfloat vertices[][3] = 
    {
        {0.0f, 0.0f, 0.0f}, 
        {1.0f, 0.0f, 0.0f}, 
    };

    createLineStrip(&vertices[0][0], 2);
}

LineStrip::~LineStrip()
{
}

void LineStrip::createLineStrip(const GLfloat* points, GLuint numPoints)
{
    GLF_ASSERT(numPoints > 1);

    GLfloat* vertices = new GLfloat [numPoints * 7];
    GLuint* indices = new GLuint [numPoints];

    if (numPoints >= 3)
    {
        // -------------------------------------------------------------- 
        // Compute the normals, texture coordinates and indices
        // -------------------------------------------------------------- 
        GLfloat length = 0;
        for (GLuint i = 0; i < numPoints; ++i)
        {
            GLfloat* v = &vertices[i * 7];

            v[0] = points[i * 3];
            v[1] = points[i * 3 + 1];
            v[2] = points[i * 3 + 2];

            glm::vec3 n;

            if (i == 0)
            {
                const GLfloat* p0 = &points[i * 3];
                const GLfloat* p1 = &points[(i + 1) * 3];
                const GLfloat* p2 = &points[(i + 2) * 3];
                
                glm::vec3 v0 = glm::vec3(p0[0], p0[1], p0[2]);
                glm::vec3 v1 = glm::vec3(p1[0], p1[1], p1[2]);
                glm::vec3 v2 = glm::vec3(p2[0], p2[1], p2[2]);

                glm::vec3 t0 = glm::normalize(v1 - v0);
                glm::vec3 t1 = glm::normalize(v2 - v0);
                glm::vec3 t = t0;

                glm::vec3 b = glm::cross(t0, t1);

                n = glm::normalize(glm::cross(t, b));
            }
            else if (i == numPoints - 1)
            {
                const GLfloat* p0 = &points[(i - 2) * 3];
                const GLfloat* p1 = &points[(i - 1) * 3];
                const GLfloat* p2 = &points[i * 3];

                glm::vec3 v0 = glm::vec3(p0[0], p0[1], p0[2]);
                glm::vec3 v1 = glm::vec3(p1[0], p1[1], p1[2]);
                glm::vec3 v2 = glm::vec3(p2[0], p2[1], p2[2]);

                glm::vec3 t0 = glm::normalize(v1 - v0);
                glm::vec3 t1 = glm::normalize(v2 - v0);
                glm::vec3 t = t1;

                glm::vec3 b = glm::cross(t0, t1);

                n = glm::normalize(glm::cross(t, b));
            }
            else
            {
                const GLfloat* p0 = &points[(i - 1) * 3];
                const GLfloat* p1 = &points[i * 3];
                const GLfloat* p2 = &points[(i + 1) * 3];
                
                glm::vec3 v0 = glm::vec3(p0[0], p0[1], p0[2]);
                glm::vec3 v1 = glm::vec3(p1[0], p1[1], p1[2]);
                glm::vec3 v2 = glm::vec3(p2[0], p2[1], p2[2]);

                glm::vec3 t0 = glm::normalize(v1 - v0);
                glm::vec3 t1 = glm::normalize(v2 - v0);
                glm::vec3 t = (t0 + t1) * 0.5f;

                glm::vec3 b = glm::cross(t0, t1);

                n = glm::normalize(glm::cross(t, b));
            }

            v[4] = n.x;
            v[5] = n.y;
            v[6] = n.z;

            if (i != numPoints - 1)
            {
                const GLfloat* p0 = &points[i * 3];
                const GLfloat* p1 = &points[(i + 1) * 3];
                length += glm::length(glm::vec3(p0[0], p0[1], p0[2]) - 
                                      glm::vec3(p1[0], p1[1], p1[2]));
            }
        }
        
        GLfloat len = 0;
        for (GLuint i = 0; i < numPoints; ++i)
        {
            GLfloat* v = &vertices[i * 8];

            GLfloat l = 0;
            if (i == 0)
            {
                v[3] = 0;
            }
            else if (i == numPoints - 1)
            {
                v[3] = 1.0f;
            }
            else
            {
                const GLfloat* p0 = &points[(i - 1) * 3];
                const GLfloat* p1 = &points[i * 3];
                len += glm::length(glm::vec3(p0[0], p0[1], p0[2]) - 
                                   glm::vec3(p1[0], p1[1], p1[2]));
                v[3] = len / length;
            }

            indices[i] = i;
        }
    }
    else
    {
        GLfloat* v = &vertices[0];
        GLuint* i = &indices[0];

        glm::vec3 d(points[3] - points[0], 
                    points[4] - points[1], 
                    points[5] - points[2]);
        d = glm::normalize(d);

        v[0] = points[0];
        v[1] = points[1];
        v[2] = points[2];
        v[3] = 0;
        v[4] = -d.y;
        v[5] = d.x;
        v[6] = 0;
        
        v[7]  = points[3];
        v[8]  = points[4];
        v[9]  = points[5];
        v[10] = 1;
        v[11] = -d.y;
        v[12] = d.x;
        v[13] = 0;
                                
        i[0] = 0;
        i[1] = 1;
    }

    // -------------------------------------------------------------- 
    // Create the vertex buffer object
    // -------------------------------------------------------------- 
    VertexAttribDescriptor desc[] =
    {
        {VERTEX_ATTRIB_POSITION, GL_FLOAT, 3}, 
        {VERTEX_ATTRIB_TEXCOORD, GL_FLOAT, 1}, 
        {VERTEX_ATTRIB_NORMAL, GL_FLOAT, 3}, 
    };

    _primitive = GL_LINE_STRIP;
    bool ret = _vertexArray.create(GL_LINE_STRIP, vertices, numPoints, &desc[0], 3, indices, numPoints);
    GLF_ASSERT(ret);
    
    delete [] vertices;
    delete [] indices;
}

GLF_NAMESPACE_END
