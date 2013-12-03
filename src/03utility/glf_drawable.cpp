// -------------------------------------------------------------- 
// glf_drawable.cpp
// A wrapper of geometric object with a transformation matrix.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#include "glf_drawable.h"

#include "geometry/glf_mesh.h"
#include "../01system/glf_assert.h"

GLF_NAMESPACE_BEGIN

Drawable::Drawable(const char* mesh)
{
    _geometry = new Mesh(mesh);

    _scaling = glm::vec3(1, 1, 1);
    _rotation = glm::vec3(0, 0, 0);
    _position = glm::vec3(0, 0, 0);

    _transformation = glm::mat4(1.0f);
    _dirty = false;
}

Drawable::Drawable(AbstractGeometry* geometry)
{
    GLF_ASSERT(geometry != NULL);
    _geometry = geometry;
    
    _scaling = glm::vec3(1, 1, 1);
    _rotation = glm::vec3(0, 0, 0);
    _position = glm::vec3(0, 0, 0);

    _transformation = glm::mat4(1.0f);
    _dirty = false;
}

Drawable::~Drawable()
{
    delete _geometry;
}

void Drawable::render(GLuint times)
{
    GLF_ASSERT(_geometry != NULL);
    if (_geometry != NULL)
    {
        _geometry->render(times);
    }
}

void Drawable::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    _position.x = x;
    _position.y = y;
    _position.z = z;

    _dirty = true;
}

void Drawable::setRotation(GLfloat rx, GLfloat ry, GLfloat rz)
{
    _rotation.x = rx;
    _rotation.y = ry;
    _rotation.z = rz;

    _dirty = true;
}

void Drawable::setScaling(GLfloat sx, GLfloat sy, GLfloat sz)
{
    _scaling.x = sx;
    _scaling.y = sy;
    _scaling.z = sz;

    _dirty = true;
}
    
glm::mat4 Drawable::getTransformation() const
{
    if (_dirty)
    {
        _dirty = true;

        glm::mat4 rotationx = glm::eulerAngleX(_rotation.x);
        glm::mat4 rotationy = glm::eulerAngleX(_rotation.y);
        glm::mat4 rotationz = glm::eulerAngleX(_rotation.z);

        _transformation = rotationx * rotationy * rotationz;

        _transformation[0][0] *= _scaling.x;
        _transformation[0][1] *= _scaling.x;
        _transformation[0][2] *= _scaling.x;
        
        _transformation[1][0] *= _scaling.y;
        _transformation[1][1] *= _scaling.y;
        _transformation[1][2] *= _scaling.y;
        
        _transformation[2][0] *= _scaling.z;
        _transformation[2][1] *= _scaling.z;
        _transformation[2][2] *= _scaling.z;

        _transformation[3][0] = _position.x;
        _transformation[3][1] = _position.y;
        _transformation[3][2] = _position.z;
    }

    return _transformation;
}

GLF_NAMESPACE_END
