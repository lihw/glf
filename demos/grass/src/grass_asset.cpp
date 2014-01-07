// grass_asset.cpp
// The grass geometry and texture
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "grass_asset.h"

#include <glf/glf.h>

const GLuint TOTAL_NUM_BLADES = 400;

const GLfloat WIDTH = 10.0f;
const GLfloat HEIGHT = 10.0f;

GrassAsset::GrassAsset(GLfloat bladeLength)
{
    m_vertices = NULL;
    m_indices = NULL;
    m_bladeLength = bladeLength;

    if (loadBladeAsset("blade.txt"))
    {
        m_numBlades = TOTAL_NUM_BLADES;

        srand(1001);

        GLfloat *seeds = plantSeeds(m_numBlades);
    
        m_numVertices = m_numBlades * NUM_VERTICES_PER_BLADE;
        m_vertices = new Vertex [m_numVertices];
    
        GLuint numBladeIndices = (NUM_VERTICES_PER_BLADE - 1) * 2;
        m_numIndices = m_numBlades * numBladeIndices;
        m_indices = new GLuint [m_numIndices];

        GLfloat* seed    = seeds;
        Vertex* vertices = m_vertices;
        GLuint* indices  = m_indices;
        GLuint baseIndex = 0;
        for (GLuint i = 0; i < TOTAL_NUM_BLADES; ++i)
        {
            growBlade(seed, baseIndex, vertices, indices);

            seed += 2;
            vertices += NUM_VERTICES_PER_BLADE;
            indices += numBladeIndices;
            baseIndex += NUM_VERTICES_PER_BLADE;
        }

        delete [] seeds;

        // -------------------------------------------------------------- 
        // Vertex attribute
        // -------------------------------------------------------------- 
        m_vertexDesc[0].position = glf::VERTEX_ATTRIB_POSITION;
        m_vertexDesc[0].type = GL_FLOAT;
        m_vertexDesc[0].size = 3;
        
        m_vertexDesc[1].position = glf::VERTEX_ATTRIB_NORMAL;
        m_vertexDesc[1].type = GL_FLOAT;
        m_vertexDesc[1].size = 3;
        
        m_vertexDesc[2].position = glf::VERTEX_ATTRIB_COLOR;
        m_vertexDesc[2].type = GL_FLOAT;
        m_vertexDesc[2].size = 3;

        m_vertexDesc[3].position = glf::VERTEX_ATTRIB_UNNAMED0;
        m_vertexDesc[3].type = GL_FLOAT;
        m_vertexDesc[3].size = 1;
        
        m_vertexDesc[4].position = glf::VERTEX_ATTRIB_UNNAMED1;
        m_vertexDesc[4].type = GL_FLOAT;
        m_vertexDesc[4].size = 3;
    }
    else
    {
        GLF_LOGERROR("Failed to open blade.txt");
        m_numBlades = 0;
    }
}

GrassAsset::~GrassAsset()
{
    delete [] m_vertices;
    delete [] m_indices;
}
    
GLfloat* GrassAsset::plantSeeds(GLuint numSeeds)
{
    GLfloat* seeds = new GLfloat [numSeeds * 2];
    GLfloat* s = seeds;
    for (GLuint i = 0; i < numSeeds; ++i)
    {
        GLfloat x = ((GLfloat)(i % 20) / 20.0f - 0.5f) * WIDTH;
        GLfloat y = ((GLfloat)(i / 20) / 20.0f - 0.5f) * HEIGHT;

        *s++ = x;
        *s++ = y;
    }

    return seeds;
}

bool GrassAsset::loadBladeAsset(const char* filename)
{

    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        GLF_LOGWARNING("Failed to open %s", filename);
        return false;
    }

    GLuint lineno = 1;
    for (GLuint i = 0; i < NUM_ASSET_BLADES; ++i)
    {
        char line[1024];
        if (fgets(line, 1024, fp) == NULL)
        {
            GLF_LOGWARNING("Error in reading line %d", lineno);
            return false;
        }

        const char* p = line;
        for (GLuint j  = 0; j < NUM_VERTICES_PER_BLADE; ++j)
        {
            const char *end = strchr(p, ')');
            char buffer[1024];
            strncpy(buffer, p, end - p + 1);
            buffer[end - p + 1] = 0;
            size_t n = sscanf(buffer, "(%f,%f,%f)", 
                    &m_bladeAsset[i][j].position[0],
                    &m_bladeAsset[i][j].position[1],
                    &m_bladeAsset[i][j].position[2]);

            if (n != 3)
            {
                GLF_LOGWARNING("Wrong format in line %d", lineno);
                return false;
            }
            p = end + 1;
        }

        lineno++;
    }

    fclose(fp);

    return true;
}
    
void GrassAsset::growBlade(GLfloat* position, GLuint baseIndex, Vertex* out_vertices, 
        GLuint* out_indices)
{
    // Choose a random blade
    //
    // FIXME: 3 is the number of blades in blade.txt
    GLuint randBladeIndex = rand() % 3;
    Vertex* bladeKnots = &m_bladeAsset[randBladeIndex][0];

    // A random length of the blade
    GLfloat lengthScaling = (GLfloat)rand() / (GLfloat)RAND_MAX;
    lengthScaling = lengthScaling * 0.8f + 0.5f; // (0.5, 1.3)
    lengthScaling *= m_bladeLength / 4.0f; // scale the blade length for the current scene.

    out_vertices[0].position.x = position[0];
    out_vertices[0].position.y = 0;
    out_vertices[0].position.z = position[1];

    out_vertices[0].distance = 0;
    
    // Add random orientatino to the blade
    GLfloat rotY = (GLfloat)rand() / (GLfloat)RAND_MAX;
    rotY = rotY * 2.0f - 1.0f;
    rotY *= glf::PI;
    glm::mat4 rotYMatrix = glm::eulerAngleY(rotY);
    
    // Lengthen the blade and reposition the blade to the seed.
    for (GLuint i = 1; i < NUM_VERTICES_PER_BLADE; ++i)
    {
        glm::vec3 edge = bladeKnots[i].position - bladeKnots[i - 1].position;
        glm::vec3 scaledEdge = edge * lengthScaling;
        glm::vec4 rotatedEdge = rotYMatrix * glm::vec4(scaledEdge, 1.0f);
        out_vertices[i].position = out_vertices[i - 1].position + glm::vec3(rotatedEdge.x,  rotatedEdge.y, rotatedEdge.z);

        out_vertices[i].distance += out_vertices[i - 1].distance + glm::length(scaledEdge);
    }

    // Compute the normal, tangents and other properties of each knot on the blade.
    glm::vec4 binormal = rotYMatrix * glm::vec4(0, 0, -1, 0);
    glm::vec3 b = glm::vec3(binormal.x, binormal.y, binormal.z);

    for (GLuint i = 0; i < NUM_VERTICES_PER_BLADE; ++i)
    {
        if (i == 0)
        {
            glm::vec3 t = out_vertices[1].position - out_vertices[0].position;
            out_vertices[0].normal = glm::normalize(glm::cross(t, b));
            out_vertices[0].tangent = glm::normalize(t);
        }
        else if (i == NUM_VERTICES_PER_BLADE - 1)
        {
            glm::vec3 t = out_vertices[NUM_VERTICES_PER_BLADE - 1].position - out_vertices[NUM_VERTICES_PER_BLADE - 2].position;
            out_vertices[NUM_VERTICES_PER_BLADE - 1].normal = glm::normalize(glm::cross(t, b));
            out_vertices[NUM_VERTICES_PER_BLADE - 1].tangent = glm::normalize(t);
        }
        else
        {
            glm::vec3 t0 = out_vertices[i + 1].position - out_vertices[i].position;
            glm::vec3 t1 = out_vertices[i].position - out_vertices[i - 1].position;
            glm::vec3 t = t0 + t1;
            out_vertices[i].tangent = glm::normalize(t);
            out_vertices[i].normal = glm::normalize(glm::cross(t, b));
        }

        if (i >= 1)
        {
            out_vertices[i].distance = out_vertices[i - 1].distance + glm::distance(out_vertices[i - 1].position, out_vertices[i].position);
        }
        else
        {
            out_vertices[i].distance = 0;
        }
    }

    for (GLuint i = 1; i < NUM_VERTICES_PER_BLADE - 1; ++i)
    {
        out_vertices[i].distance /= out_vertices[NUM_VERTICES_PER_BLADE - 1].distance;
    }

    out_vertices[NUM_VERTICES_PER_BLADE - 1].distance = 1.0f;

    
    // -------------------------------------------------------------- 
    // Create the indices
    // -------------------------------------------------------------- 
    for (GLuint j = 0; j < NUM_VERTICES_PER_BLADE - 1; ++j)
    {
        out_indices[j * 2] = baseIndex + j;
        out_indices[j * 2 + 1] = baseIndex + j + 1;
    }
}
