// -------------------------------------------------------------- 
// glf_geometry.h
// Geometry objects
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_geometry.h"

#include "../00system/glf_assert.h"

GLF_NAMESPACE_BEGIN

// -------------------------------------------------------------- 
// Geometry class
// -------------------------------------------------------------- 
Geometry::Geometry()
{
}

Geometry::~Geometry()
{
}

void Geometry::render(GLuint times)
{
    _vertexArray.render(times);
}


// -------------------------------------------------------------- 
// Plane class
// -------------------------------------------------------------- 
Plane::Plane()
{
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
        {VERTEX_ATTRIB_NORMAL,   GL_FLOAT, 3}, 
    };

    bool ret = _vertexArray.create(GL_TRIANGLES, &vertices[0][0], 4, &desc[0], 3, indices, 6);
    GLF_ASSERT(ret);
}

Plane::~Plane()
{
}

// -------------------------------------------------------------- 
// Sphere class
// -------------------------------------------------------------- 
Sphere::Sphere()
{
#define RING_NUMBER 16
#define SEGMENT_NUMBER 32
#define PI 3.141592653589f

    GLuint numVertices = (RING_NUMBER + 1) * (SEGMENT_NUMBER + 1);
    GLuint numIndices = 6 * RING_NUMBER * (SEGMENT_NUMBER + 1);

    GLfloat* vertexBuffer = new GLfloat [numVertices * 8];
    GLuint* indexBuffer = new GLuint [numIndices]; 

    GLfloat* pVertex = vertexBuffer;
    GLuint* pIndex = indexBuffer;

    GLfloat deltaRingAngle = PI / RING_NUMBER;
    GLfloat deltaSegAngle = (2.0f * PI / SEGMENT_NUMBER);

    GLuint verticeIndex = 0;

    // Generate the group of rings for the sphere
    for (GLuint ring = 0; ring <= RING_NUMBER; ++ring) 
    {
        GLfloat r0 = sinf(ring * deltaRingAngle);
        GLfloat y0 = cosf(ring * deltaRingAngle);

        // Generate the group of segments for the current ring
        for(GLuint seg = 0; seg <= SEGMENT_NUMBER; seg++) 
        {
            GLfloat x0 = r0 * sinf(seg * deltaSegAngle);
            GLfloat z0 = r0 * cosf(seg * deltaSegAngle);

            // Add one vertex to the strip which makes up the sphere
            
            // Position.
            *pVertex++ = x0;
            *pVertex++ = y0;
            *pVertex++ = z0;

            // Texture coordinate
            *pVertex++ = (GLfloat)seg / (GLfloat)SEGMENT_NUMBER;
            *pVertex++ = (GLfloat)ring / (GLfloat)RING_NUMBER;
            
            // Normal
            glm::vec3 n(x0, y0, z0);
            n = glm::normalize(n);
            *pVertex++ = n[0];
            *pVertex++ = n[1];
            *pVertex++ = n[2];

            if (ring != RING_NUMBER) 
            {
                // each vertex (except the last) has six indices pointing to it
                *pIndex++ = verticeIndex + SEGMENT_NUMBER + 1;
                *pIndex++ = verticeIndex;               
                *pIndex++ = verticeIndex + SEGMENT_NUMBER;
                *pIndex++ = verticeIndex + SEGMENT_NUMBER + 1;
                *pIndex++ = verticeIndex + 1;
                *pIndex++ = verticeIndex;
                verticeIndex++;
            }
        } // end for seg
    } // end for ring
    
    VertexAttribDescriptor desc[] =
    {
        {VERTEX_ATTRIB_POSITION, GL_FLOAT, 3, },  // position
        {VERTEX_ATTRIB_TEXCOORD,  GL_FLOAT, 2, },  // texture coordinate
        {VERTEX_ATTRIB_NORMAL,   GL_FLOAT, 3, },  // normal
    };
    
    bool ret = _vertexArray.create(GL_TRIANGLES, vertexBuffer, numVertices, &desc[0], 3, indexBuffer, numIndices);
    GLF_ASSERT(ret);

#undef RING_NUMBER
#undef SEGMENT_NUMBER
#undef PI
}

Sphere::~Sphere()
{
}

// -------------------------------------------------------------- 
// Cube class
// -------------------------------------------------------------- 

GLF_NAMESPACE_END
