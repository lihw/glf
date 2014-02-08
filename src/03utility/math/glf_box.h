// -------------------------------------------------------------- 
// glf_box.h
// The axis-aligned bounding box
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_BOX_H
#define GLF_BOX_H


#include "../../glf_common.h"

GLF_NAMESPACE_BEGIN

class Box
{
public:
    Box();
    Box(const Box& other);
    ~Box();
    void operator=(const Box& other);

    void setMinMax(const glm::vec3& min, const glm::vec3& max);

    GLF_INLINE const glm::vec3& min() const { return _min; }
    GLF_INLINE const glm::vec3& max() const { return _max; }
    GLF_INLINE GLfloat width() const   { return _max.x - _min.x; }
    GLF_INLINE GLfloat height() const  { return _max.y - _min.y; }
    GLF_INLINE GLfloat depth() const   { return _max.z - _min.z; }

    // Transform the box with an affine transformation matrix.
    void transform(const glm::mat4 &matrix);

private:
    glm::vec3 _min;
    glm::vec3 _max;
};


GLF_NAMESPACE_END


#endif // !GLF_BOX_H
