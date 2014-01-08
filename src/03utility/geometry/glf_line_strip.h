// -------------------------------------------------------------- 
// glf_linestrip.h
// The geometry of a line strip
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_LINESTRIP_H
#define GLF_LINESTRIP_H

#include "glf_abstract_geometry.h"

GLF_NAMESPACE_BEGIN

class LineStrip : public AbstractGeometry
{
public:
    LineStrip(const GLfloat* points, GLuint numPoints);
    LineStrip(); // (0, 0, 0)->(1, 0, 0)

    virtual ~LineStrip();

private:
    void createLineStrip(const GLfloat* points, GLuint numPoints);
};

GLF_NAMESPACE_END

#endif // !GLF_LINESTRIP_H

