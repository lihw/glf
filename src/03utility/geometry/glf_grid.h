// -------------------------------------------------------------- 
// glf_grid.h
// The grid
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_GRID_H
#define GLF_GRID_H

#include "glf_abstract_geometry.h"

GLF_NAMESPACE_BEGIN

// A grid plane on the y = 0 with specified width and height
class Grid : public AbstractGeometry
{
public:
    Grid(GLuint hres, GLuint vres, GLfloat w, GLfloat h);
    virtual ~Grid();

private:
    
};

GLF_NAMESPACE_END


#endif // !GLF_GRID_H
