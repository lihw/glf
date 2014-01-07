// -------------------------------------------------------------- 
// glf_vertex_array.cpp
// The vertex array wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_vertex_array.h"

#include "glf_glerror.h"

#include "../01system/glf_assert.h"
#include "../01system/glf_log.h"

GLF_NAMESPACE_BEGIN

#if !defined BUFFER_OFFSET
# define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif
    
VertexArray::VertexArray()
{
    _vertexArray = 0;

    _vertexBuffer    = 0;
    _indexBuffer     = 0;

    _primitive = GL_TRIANGLES;

    _count = 0;
}

VertexArray::~VertexArray()
{
    if (_vertexArray != 0)
    {
        glDeleteVertexArrays(1, &_vertexArray);
    }
    if (_vertexBuffer != 0)
    {
        glDeleteBuffers(1, &_vertexBuffer);
    }
    if (_indexBuffer != 0)
    {
        glDeleteBuffers(1, &_indexBuffer);
    }
}

bool VertexArray::create(GLenum pritimive,
                         const GLfloat* vertices,
                         GLuint numVertices,
                         const VertexAttribDescriptor* vertexAttribs,
                         GLuint numVertexAttribs,
                         GLuint* vertexIndices,
                         GLuint numIndices)
{
    if (_vertexArray != 0)
    {
        GLF_LOGWARNING("Vertex array is already created");
        return false;
    }

    _primitive = pritimive;

    glGenVertexArrays(1, &_vertexArray);
    glBindVertexArray(_vertexArray);

    // -------------------------------------------------------------- 
    // Set up vertex attributes
    // -------------------------------------------------------------- 
    
    // Calculate the size of one vertex
    GLuint vertexSize = 0;
    for (GLuint i = 0; i < numVertexAttribs; ++i)
    {
        vertexSize += vertexAttribs[i].size * getSizeOf(vertexAttribs[i].type);
    }

    GLuint offset = 0;
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, vertices, GL_STATIC_DRAW);
    offset = 0;
   
    for (GLuint i = 0; i < numVertexAttribs; ++i)
    {
        glVertexAttribPointer(vertexAttribs[i].position, 
                              vertexAttribs[i].size, 
                              vertexAttribs[i].type, 
                              GL_FALSE, 
                              vertexSize, 
                              BUFFER_OFFSET(offset));
        glEnableVertexAttribArray(vertexAttribs[i].position);
        offset += getSizeOf(vertexAttribs[i].type) * vertexAttribs[i].size;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    _number = numVertices;

    // -------------------------------------------------------------- 
    // Set up index buffer
    // -------------------------------------------------------------- 
    if (vertexIndices != NULL)
    {
        glGenBuffers(1, &_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), vertexIndices, GL_STATIC_DRAW);

        _number = numIndices;
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindVertexArray(0);

    if (!checkGLError(__FUNCTION__))
    {
        return false;
    }

    return true;
}
    
void VertexArray::updateVertices(const GLfloat* vertices,
                                 GLuint numVertices,
                                 const VertexAttribDescriptor* vertexAttribs,
                                 GLuint numVertexAttribs)
{
    GLuint vertexSize = 0;
    for (GLuint i = 0; i < numVertexAttribs; ++i)
    {
        vertexSize += vertexAttribs[i].size * getSizeOf(vertexAttribs[i].type);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::render(GLuint times)
{
    if (_vertexArray)
    {
        glBindVertexArray(_vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        
        if (_indexBuffer != 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
            glDrawElementsInstanced(_primitive, _number, GL_UNSIGNED_INT, NULL, times);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            glDrawArraysInstanced(_primitive, 0, _number, times);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        checkGLError(__FUNCTION__);
    }
}

GLuint VertexArray::getSizeOf(GLenum type)
{
    switch (type)
    {
        case GL_FLOAT: 
        case GL_UNSIGNED_INT:
        case GL_INT: 
            return 4;
            break;
        case GL_UNSIGNED_SHORT:
        case GL_SHORT:
            return 2;
            break;
        case GL_UNSIGNED_BYTE:
        case GL_BYTE:
            return 2;
            break;
        default:
            GLF_ASSERT_NOT_REACHABLE();
            break;
    }

    return 0;
}

GLF_NAMESPACE_END
