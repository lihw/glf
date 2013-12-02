// -------------------------------------------------------------- 
// glf_vertex_array.h
// The vertex array wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#ifndef GLF_VERTEX_ARRAY_H
#define GLF_VERTEX_ARRAY_H


#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

// The vertex attribute layout position
enum VertexAttribPositionEnum
{
    VERTEX_ATTRIB_POSITION  = 0,
    VERTEX_ATTRIB_NORMAL    = 1,
    VERTEX_ATTRIB_TEXCOORD  = 2,
    VERTEX_ATTRIB_COLOR     = 3,
    VERTEX_ATTRIB_UNAMED0   = 4,
    VERTEX_ATTRIB_UNAMED1   = 5,
    VERTEX_ATTRIB_UNAMED2   = 6,
    VERTEX_ATTRIB_UNAMED3   = 7,
    VERTEX_ATTRIB_UNAMED4   = 8,

    VERTEX_ATTRIB_FIRST = VERTEX_ATTRIB_POSITION,
    VERTEX_ATTRIB_LAST = VERTEX_ATTRIB_UNAMED4,
    VERTEX_ATTRIB_NUMBER = VERTEX_ATTRIB_LAST - VERTEX_ATTRIB_FIRST + 1,
};

struct VertexAttribDescriptor
{
    VertexAttribPositionEnum  position;
    GLenum                    type;
    GLuint                    size;
};

class VertexArray
{
    VertexArray(const VertexArray& other) {}
    void operator=(const VertexArray& other) {}

public:
    VertexArray();
    ~VertexArray();

    // When \param vertexIndices is NULL, numIndices points to the number of
    // vertices.
    bool create(GLenum pritimive,
                const GLfloat* vertices,
                GLuint numVertices,
                VertexAttribDescriptor* vertexAttribs,
                GLuint numVertexAttribs,
                GLuint* vertexIndices,
                GLuint numIndices);

    void render(GLuint times);

    GLF_INLINE GLuint getVertexArrayHandle() const
    { return _vertexArray; }

private:
    GLuint getSizeOf(GLenum type);

private:
    GLuint _vertexArray;

    GLuint _vertexBuffer;
    GLuint _indexBuffer;
    GLenum _primitive;
    GLuint _count;
    GLuint _number;
};

GLF_NAMESPACE_END


#endif // !GLF_VERTEX_ARRAY_H

