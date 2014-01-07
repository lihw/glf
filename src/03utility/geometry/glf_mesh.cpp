// -------------------------------------------------------------- 
// glf_mesh.cpp
// Mesh importer (handle a variant of 3D formats)
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#include "glf_mesh.h"

#include <cstdio>
#include <cstring>
#include <string>

#include "../../01system/glf_log.h"
#include "../../01system/glf_assert.h"

// Model loader headers
#include "mesh/glm.h" // OBJ file
#include "mesh/Model_3DS.h" // 3DS file

GLF_NAMESPACE_BEGIN

Mesh::Mesh(const char* filename)
{
    const char* suffix = strrchr(filename, '.');
    if (suffix == NULL)
    {
        GLF_LOGERROR("Failed to detect the mesh file type");
        throw std::string("Failed to detect the mesh file type");
    }
    else
    {
        GLfloat*  vertices;  
        GLuint*   indices;     
        GLuint    nvertices; 
        GLuint    nindices;    
        bool      hasNormal;
        bool      hasTexcoord;

        suffix++;

        bool succeed = false;
        if (strncmp(suffix, "ply", 3) == 0)
        {
            succeed = loadPly(filename, vertices, nvertices, indices, nindices, 
                hasNormal, hasTexcoord);
        }
        else if (strncmp(suffix, "obj", 3) == 0)
        {
            succeed = loadObj(filename, vertices, nvertices, indices, nindices, 
                hasNormal, hasTexcoord);
        }
        else if (strncmp(suffix, "3ds", 3) == 0)
        {
            succeed = load3ds(filename, vertices, nvertices, indices, nindices, 
                hasNormal, hasTexcoord);
        }
        else
        {
            GLF_LOGERROR("Unsupported mesh file type");
            throw std::string("Unsupported mesh file type");
        }

        if (succeed)
        {
            createMesh(vertices, nvertices, indices, nindices, hasNormal, hasTexcoord);
        }
        else
        {
            throw std::string("Failed to read mesh file");
        }

        delete [] vertices;
        delete [] indices;
    }
}
    
Mesh::Mesh(GLfloat* vertices, GLuint numVertices,
           GLuint* indices, GLuint numIndices,
           GLenum primitive,
           const VertexAttribDescriptor* desc,
           GLuint numDescEntries)
{
    GLF_ASSERT(vertices != NULL && numVertices > 0);
    GLF_ASSERT(indices != NULL && numIndices > 0);
    GLF_ASSERT(numDescEntries >= 1);

    _primitive = primitive;

    bool ret = _vertexArray.create(primitive, 
            vertices, 
            numVertices, 
            desc, 
            numDescEntries, 
            indices, 
            numIndices);
    GLF_ASSERT(ret);
}

Mesh::~Mesh()
{
}

void Mesh::createMesh(GLfloat* vertices,
                GLuint numVertices,
                GLuint* indices,
                GLuint numIndices,
                bool hasNormal,
                bool hasTexcoord)
{
    GLF_ASSERT(vertices != NULL && numVertices > 0);
    GLF_ASSERT(indices != NULL && numIndices > 0);

    VertexAttribDescriptor desc[3];

    GLuint ndesc = 1;
    desc[0].position = VERTEX_ATTRIB_POSITION;
    desc[0].type = GL_FLOAT;
    desc[0].size = 3;

    if (hasNormal)
    {
        desc[1].position = VERTEX_ATTRIB_NORMAL;
        desc[1].type = GL_FLOAT;
        desc[1].size = 3;

        ndesc++;
    }
    
    if (hasTexcoord)
    {
        desc[2].position = VERTEX_ATTRIB_TEXCOORD;
        desc[2].type = GL_FLOAT;
        desc[2].size = 2;
        
        ndesc++;
    }

    bool ret = _vertexArray.create(GL_TRIANGLES, 
            vertices, 
            numVertices, 
            &desc[0], 
            ndesc, 
            indices, 
            numIndices);

    GLF_ASSERT(ret);
}

bool Mesh::load3ds(const char* filename, GLfloat*& vertices, GLuint& numVertices,
            GLuint*& indices, GLuint& numIndices, bool& hasNormal, bool& hasTexcoord)
{
    return false;
}

bool Mesh::loadObj(const char* filename, GLfloat*& vertices, GLuint& numVertices,
            GLuint*& indices, GLuint& numIndices, bool& hasNormal, bool& hasTexcoord)
{
    GLMmodel* model = glmReadOBJ((char* )filename);
    if (model == NULL)
    {
        return NULL;
    }

    // Normalize the model
    glmUnitize(model);

    numVertices = model->numvertices;
    numIndices = model->numtriangles * 3;

    GLuint vertexSize = 3;
    if (model->numnormals)
    {
        hasNormal = true;
        vertexSize += 3;
    }
    if (model->numtexcoords)
    {
        hasTexcoord = true;
        vertexSize += 2;
    }
    
    vertices = new GLfloat [vertexSize * numVertices];
    // We suppose the normal index and texture coordinate
    // index are the same as vertex index. 
    for (GLuint i = 0; i < numVertices; ++i)
    {
        GLfloat* v = &vertices[i * vertexSize];

        // Note that glm is indexed from 1 rather 0. We have to convert
        // it to index from 0.
        v[0] = model->vertices[i * 3 + 3];
        v[1] = model->vertices[i * 3 + 4];
        v[2] = model->vertices[i * 3 + 5];
        
        if (hasNormal)
        {
            v[3] = model->normals[i * 3 + 3];
            v[4] = model->normals[i * 3 + 4];
            v[5] = model->normals[i * 3 + 5];
        }
        
        if (hasTexcoord)
        {
            v[6] = model->texcoords[i * 2 + 2];
            v[7] = model->texcoords[i * 2 + 3];
        }
    }

    indices = new GLuint [3 * numIndices];
    for (GLuint i = 0 ; i < model->numtriangles; i++)
    {
        int j = i * 3;
        indices[j + 0] = model->triangles[i].vindices[0] - 1;
        indices[j + 1] = model->triangles[i].vindices[1] - 1;
        indices[j + 2] = model->triangles[i].vindices[2] - 1;
    }

    glmDelete(model);

    return true;
}

bool Mesh::loadPly(const char* filename, GLfloat*& vertices, GLuint& numVertices,
            GLuint*& indices, GLuint& numIndices, bool& hasNormal, bool& hasTexcoord)
{
    return false;
}
    
void Mesh::updateVertices(GLfloat* vertices, GLuint numVertices,
        const VertexAttribDescriptor* vertexAttribs, GLuint numVertexAttribs)
{
    _vertexArray.updateVertices(vertices, numVertices, vertexAttribs, numVertexAttribs);
}

GLF_NAMESPACE_END
