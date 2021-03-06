// -------------------------------------------------------------- 
// glf_mesh.h
// Mesh importer (handle a variant of 3D formats)
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_MESH_H
#define GLF_MESH_H

#include "glf_abstract_geometry.h"

#include "../../02opengl/glf_vertex_array.h"

GLF_NAMESPACE_BEGIN

class Mesh : public AbstractGeometry
{
public:
    Mesh(const char* filename);
    Mesh(GLfloat* vertices, GLuint numVertices,
         GLuint* indices, GLuint numIndices,
         GLenum primitive,
         const VertexAttribDescriptor* desc,
         GLuint numDescEntries);
    virtual ~Mesh();
    
    virtual void updateVertices(GLfloat* vertices, GLuint numVertices,
        const VertexAttribDescriptor* vertexAttribs, GLuint numVertexAttribs);

private:
    bool loadObj(const char* filename, GLfloat*& vertices, GLuint& numVertices,
            GLuint*& indices, GLuint& numIndices, bool& hasNormal, bool& hasTexcoord);
    bool load3ds(const char* filename, GLfloat*& vertices, GLuint& numVertices,
            GLuint*& indices, GLuint& numIndices, bool& hasNormal, bool& hasTexcoord);
    bool loadPly(const char* filename, GLfloat*& vertices, GLuint& numVertices,
            GLuint*& indices, GLuint& numIndices, bool& hasNormal, bool& hasTexcoord);

    void createMesh(GLfloat* vertices,
                    GLuint numVertices,
                    GLuint* indices,
                    GLuint numIndices,
                    bool hasNormal,
                    bool hasTexcoord);
};

GLF_NAMESPACE_END

#endif // !GLF_MESH_H
