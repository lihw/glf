// grass_asset.cpp
// The grass geometry and texture
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "grass_asset.h"

#include <glf/glf.h>

const GLuint TOTAL_NUM_BLADES = 400;
const GLuint NUM_VERTICES_PER_BLADE = 8;

const GLfloat WIDTH = 10.0f;
const GLfloat HEIGHT = 10.0f;

GrassAsset::GrassAsset()
{
    m_vertices = NULL;
    m_indices = NULL;
    m_numBlades = TOTAL_NUM_BLADES;

    GLfloat* seeds = createSeeds();
    createBlades(seeds);
}

GrassAsset::~GrassAsset()
{
    delete [] m_vertices;
    delete [] m_indices;
}
    
GLfloat* GrassAsset::createSeeds()
{
    GLfloat* seeds = new GLfloat [m_numBlades * 2];
    GLfloat* s = seeds;
    for (GLuint i = 0; i < m_numBlades; ++i)
    {
        GLfloat x = ((GLfloat)(i % 20) / 20.0f - 0.5f) * WIDTH;
        GLfloat y = ((GLfloat)(i / 20) / 20.0f - 0.5f) * HEIGHT;

        *s++ = x;
        *s++ = y;
    }

    return seeds;
}

void GrassAsset::createBlades(GLfloat* seeds)
{
    srand(1001);

    const GLfloat gravity[] =
    {
        0,
        -9.8337f,
        0
    };

    // -------------------------------------------------------------- 
    // Create the vertices
    // -------------------------------------------------------------- 
    m_numVertices = m_numBlades * NUM_VERTICES_PER_BLADE;
    m_vertices = new GrassBladeVertex [m_numVertices];

    for (GLuint i = 0; i < m_numBlades; ++i)
    {
        // -------------------------------------------------------------- 
        // A random trajectory
        // -------------------------------------------------------------- 
        GLfloat trajectory[3];

        // Sample the top half of hemisphere to the trajectory direction
        trajectory[1] = 1.0f - (GLfloat)rand() / RAND_MAX * 0.9999f * 0.5f;
        GLfloat theta = (GLfloat)rand() / RAND_MAX * 2.0f * GLF_PI;
        GLfloat r = sqrtf(1.0f - trajectory[1] * trajectory[1]);
        trajectory[2] = r * cosf(theta);
        trajectory[0] = r * sinf(theta);
        
        // A random trajectory speed value.
        GLfloat speed = (GLfloat)rand() / (GLfloat)RAND_MAX;
        speed = (speed * 0.5f + 0.5f) * 5.0f;

        trajectory[0] *= speed;
        trajectory[1] *= speed;
        trajectory[2] *= speed;

        GLfloat x = seeds[i * 2];
        GLfloat z = seeds[i * 2 + 1];

        // -------------------------------------------------------------- 
        // Grow the grass blade along the trajectory direction and affected by the
        // gravity.
        // -------------------------------------------------------------- 
        GrassBladeVertex* bladeVertices = &m_vertices[i * NUM_VERTICES_PER_BLADE];
        bladeVertices[0].position[0] = x;
        bladeVertices[0].position[1] = 0;
        bladeVertices[0].position[2] = z;

        bladeVertices[0].tangent[0] = trajectory[0] / speed;
        bladeVertices[0].tangent[1] = trajectory[1] / speed;
        bladeVertices[0].tangent[2] = trajectory[2] / speed;

        if (trajectory[0] != 0)
        {
            bladeVertices[0].normal[0] = -bladeVertices[0].tangent[1];
            bladeVertices[0].normal[1] = bladeVertices[0].tangent[0];
            bladeVertices[0].normal[2] = 0;
        }
        else
        {
            bladeVertices[0].normal[0] = 0;
            bladeVertices[0].normal[1] = -bladeVertices[0].tangent[2];
            bladeVertices[0].normal[2] = bladeVertices[0].tangent[1];
        }

        bladeVertices[0].distance = 0;

        GLfloat velocity[] = 
        {
            trajectory[0],
            trajectory[1],
            trajectory[2],
        };

        GLfloat deltaTime = 1.0f / (GLfloat)NUM_VERTICES_PER_BLADE * 0.4f;

        for (GLuint j = 1; j < NUM_VERTICES_PER_BLADE; j++)
        {
            bladeVertices[j].position[0] = 
                bladeVertices[j - 1].position[0] + velocity[0] * deltaTime;
            bladeVertices[j].position[1] = 
                bladeVertices[j - 1].position[1] + velocity[1] * deltaTime;
            bladeVertices[j].position[2] = 
                bladeVertices[j - 1].position[2] + velocity[2] * deltaTime;
        
            velocity[0] += gravity[0] * deltaTime;
            velocity[1] += gravity[1] * deltaTime;
            velocity[2] += gravity[2] * deltaTime;
            
            glm::vec3 t = glm::normalize(glm::vec3(velocity[0], velocity[1], velocity[2]));
            bladeVertices[j].tangent[0] = t.x;
            bladeVertices[j].tangent[1] = t.y;
            bladeVertices[j].tangent[2] = t.z;

            glm::vec3 n = glm::vec3(bladeVertices[j - 1].normal[0],
                                    bladeVertices[j - 1].normal[1],
                                    bladeVertices[j - 1].normal[2]);

            glm::vec3 b = glm::cross(n, t);
            n = glm::normalize(glm::cross(t, b));

            bladeVertices[j].normal[0] = n.x;
            bladeVertices[j].normal[1] = n.y;
            bladeVertices[j].normal[2] = n.z;
        
            bladeVertices[j].distance = (GLfloat)(j) / (GLfloat)(NUM_VERTICES_PER_BLADE - 1);
        }
    }

    // -------------------------------------------------------------- 
    // Create the indices
    // -------------------------------------------------------------- 
    GLuint numBladeIndices = (NUM_VERTICES_PER_BLADE - 1) * 2;
    m_numIndices = m_numBlades * numBladeIndices;
    m_indices = new GLuint [m_numIndices];
    for (GLuint i = 0; i < m_numBlades; ++i)
    {
        GLuint* bladeIndices = &m_indices[i * numBladeIndices];
        GLuint blade = i * NUM_VERTICES_PER_BLADE;
        for (GLuint j = 0; j < NUM_VERTICES_PER_BLADE - 1; ++j)
        {
            bladeIndices[j * 2] = blade + j;
            bladeIndices[j * 2 + 1] = blade + j + 1;
        }
    }

    // -------------------------------------------------------------- 
    // Vertex attribute
    // -------------------------------------------------------------- 
    m_vertexDesc[0].position = glf::VERTEX_ATTRIB_POSITION;
    m_vertexDesc[0].type = GL_FLOAT;
    m_vertexDesc[0].size = 3;
    
    m_vertexDesc[1].position = glf::VERTEX_ATTRIB_NORMAL;
    m_vertexDesc[1].type = GL_FLOAT;
    m_vertexDesc[1].size = 3;

    m_vertexDesc[2].position = glf::VERTEX_ATTRIB_UNAMED1;
    m_vertexDesc[2].type = GL_FLOAT;
    m_vertexDesc[2].size = 3;
    
    m_vertexDesc[3].position = glf::VERTEX_ATTRIB_COLOR;
    m_vertexDesc[3].type = GL_FLOAT;
    m_vertexDesc[3].size = 3;
    
    m_vertexDesc[4].position = glf::VERTEX_ATTRIB_UNAMED0;
    m_vertexDesc[4].type = GL_FLOAT;
    m_vertexDesc[4].size = 1;
}
