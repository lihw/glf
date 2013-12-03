// -------------------------------------------------------------- 
// glf_drawable.h
// A wrapper of geometric object with a transformation matrix.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_DRAWABLE_H
#define GLF_DRAWABLE_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

class AbstractGeometry;

class Drawable
{
public:
    // mesh file path. 
    // torus, etc.
    Drawable(const char* mesh);
    Drawable(AbstractGeometry* geometry);
    ~Drawable();

    void render(GLuint times);

    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    // rotation order rx * ry * rz 
    void setRotation(GLfloat rx, GLfloat ry, GLfloat rz);
    void setScaling(GLfloat sx, GLfloat sy, GLfloat sz);

    glm::vec3 getPosition() const
    { return _position; }
    glm::vec3 getScaling() const
    { return _scaling; }
    glm::vec3 getRotation() const
    { return _rotation; }
    
    glm::mat4 getTransformation() const;

private:
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scaling;
    mutable bool _dirty;
    mutable glm::mat4 _transformation;

    AbstractGeometry* _geometry;
};


GLF_NAMESPACE_END

#endif // !GLF_DRAWABLE_H
