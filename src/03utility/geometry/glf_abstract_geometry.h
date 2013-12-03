// -------------------------------------------------------------- 
// glf_abstract_geometry.h
// Default geometry objects
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_ABSTRACT_GEOMETRY_H
#define GLF_ABSTRACT_GEOMETRY_H

#include "../../02opengl/glf_vertex_array.h"

GLF_NAMESPACE_BEGIN

class AbstractGeometry
{
public:
    AbstractGeometry();
    virtual ~AbstractGeometry();

    virtual void render(GLuint times);

protected:
    VertexArray   _vertexArray;
    GLenum        _primitive;
};

GLF_NAMESPACE_END

#endif // !GLF_ABSTRACT_GEOMETRY_H
