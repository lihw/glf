// grass_asset.h
// The grass geometry and texture
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#ifndef GRASS_ASSET_H
#define GRASS_ASSET_H

#include <glf/glf.h>



class GrassAsset 
{
public:
    GrassAsset();
    ~GrassAsset();

    GLF_INLINE GLuint getNumBlades() const 
    { return m_numBlades; }
    GLF_INLINE GLuint getNumVertices() const 
    { return m_numVertices; }
    GLF_INLINE GLfloat* getVertices() const 
    { return (GLfloat*)m_vertices; }
    GLF_INLINE GLuint getNumIndices() const 
    { return m_numIndices; }
    GLF_INLINE GLuint* getIndices() const 
    { return m_indices; }

    GLF_INLINE GLenum getPrimitive() const
    { return GL_LINES; }
    GLF_INLINE const glf::VertexAttribDescriptor* getVertexDesc() const
    { return &m_vertexDesc[0]; }
    GLF_INLINE GLuint getNumVertexDescEntries() const
    { return 5; }

    enum
    {
        NUM_ASSET_BLADES = 10,
        NUM_VERTICES_PER_BLADE = 8,
    };

private:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        GLfloat distance;
        glm::vec3 tangent;
    };

private:
    GLuint                      m_numBlades;
    Vertex*                     m_vertices;
    GLuint                      m_numVertices;
    GLuint*                     m_indices;
    GLuint                      m_numIndices;
    glf::VertexAttribDescriptor m_vertexDesc[5];
    Vertex                      m_bladeAsset[NUM_ASSET_BLADES][NUM_VERTICES_PER_BLADE];

private:
    void loadBladeAsset(const char* filename);
    GLfloat* plantSeed(GLuint numBlades);
    void growBlade(GLfloat* position, GLuint baseIndex, Vertex* out_vertices, 
        GLuint* out_indices);
};





#endif // !GRASS_ASSET_H
