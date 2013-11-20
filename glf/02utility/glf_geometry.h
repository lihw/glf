// -------------------------------------------------------------- 
// glf_geometry.h
// Geometry objects
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_GEOMETRY_H
#define GLF_GEOMETRY_H

#include "../01api/glf_vertex_array.h"

GLF_NAMESPACE_BEGIN

class Geometry
{
public:
    Geometry();
    virtual ~Geometry();

    void render(GLuint times);

protected:
    VertexArray   _vertexArray;
};

class Plane : public Geometry
{
public:
    Plane();
    virtual ~Plane();
};

class Cube : public Geometry
{
public:
    Cube();
    virtual ~Cube();
};

class Sphere : public Geometry
{
public:
    Sphere();
    virtual ~Sphere();
};

// TODO:
// Cylinder
// Torous
// Plane grid



GLF_NAMESPACE_END

#endif // !GLF_GEOMETRY_H
