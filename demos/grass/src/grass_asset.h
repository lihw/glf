// grass_asset.h
// The grass geometry and texture
// Created at 2013/11/25
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#ifndef GRASS_ASSET_H
#define GRASS_ASSET_H

#include <glf/glf.h>


struct GrassBladeVertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat color[3];
    GLfloat distance; // The normalized distance from the root of the blade
};

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
    { return 4; }

private:
    GLuint            m_numBlades;
    GrassBladeVertex* m_vertices;
    GLuint            m_numVertices;
    GLuint*           m_indices;
    GLuint            m_numIndices;
    glf::VertexAttribDescriptor m_vertexDesc[4];

private:
    GLfloat* createSeeds();
    void createBlades(GLfloat* seeds);
};





#endif // !GRASS_ASSET_H
