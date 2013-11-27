
// -------------------------------------------------------------- 
// glf_quad.h
// The geometry of a quad
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_QUAD_H
#define GLF_QUAD_H

#include "glf_abstract_geometry.h"

GLF_NAMESPACE_BEGIN

class Quad : public AbstractGeometry
{
public:
    Quad();
    virtual ~Quad();
};

GLF_NAMESPACE_END

#endif // !GLF_QUAD_H

