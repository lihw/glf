// -------------------------------------------------------------- 
// glf_abstract_geometry.cpp
// AbstractGeometry objects
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_abstract_geometry.h"

GLF_NAMESPACE_BEGIN

// -------------------------------------------------------------- 
// AbstractGeometry class
// -------------------------------------------------------------- 
AbstractGeometry::AbstractGeometry()
{
}

AbstractGeometry::~AbstractGeometry()
{
}

void AbstractGeometry::render(GLuint times)
{
    _vertexArray.render(times);
}
    
void AbstractGeometry::updateVertices(GLfloat* vertices, GLuint numVertices,
    const VertexAttribDescriptor* vertexAttribs, GLuint numVertexAttribs)
{
}

GLF_NAMESPACE_END
