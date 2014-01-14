// -------------------------------------------------------------- 
// glf_box.cpp
// The axis-aligned bounding box
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#include "glf_box.h"

#include <algorithm>


GLF_NAMESPACE_BEGIN


Box::Box()
{
    _min = glm::vec3(-1.0f, -1.0f, -1.0f);
    _max = glm::vec3(1.0f, 1.0f, 1.0f);
}

Box::~Box()
{
}
    
Box::Box(const Box& other)
{
    _min = other._min;
    _max = other._max;
}

void Box::operator=(const Box& other)
{
    if (this != &other)
    {
        _min = other._min;
        _max = other._max;
    }
}

void Box::setMinMax(const glm::vec3& min, const glm::vec3& max)
{
    _min = min;
    _max = max;
}

void Box::transform(const glm::mat4 &matrix)
{
    // Get the 8 corners of the cube and do the transform
    glm::vec4 corners[8];

    corners[0].x = _min.x; corners[0].y = _min.y; corners[0].z = _min.z; corners[0].w = 1.0f;
    corners[1].x = _max.x; corners[1].y = _min.y; corners[1].z = _min.z; corners[1].w = 1.0f;
    corners[2].x = _min.x; corners[2].y = _max.y; corners[2].z = _min.z; corners[2].w = 1.0f;
    corners[3].x = _max.x; corners[3].y = _max.y; corners[3].z = _min.z; corners[3].w = 1.0f;
    corners[4].x = _min.x; corners[4].y = _min.y; corners[4].z = _max.z; corners[4].w = 1.0f;
    corners[5].x = _max.x; corners[5].y = _min.y; corners[5].z = _max.z; corners[5].w = 1.0f;
    corners[6].x = _min.x; corners[6].y = _max.y; corners[6].z = _max.z; corners[6].w = 1.0f;
    corners[7].x = _max.x; corners[7].y = _max.y; corners[7].z = _max.z; corners[7].w = 1.0f;
    
    corners[0] = matrix * corners[0];
    corners[1] = matrix * corners[1];
    corners[2] = matrix * corners[2];
    corners[3] = matrix * corners[3];
    corners[4] = matrix * corners[4];
    corners[5] = matrix * corners[5];
    corners[6] = matrix * corners[6];
    corners[7] = matrix * corners[7];

    _min.x = corners[0].x; _min.y = corners[0].y; _min.z = corners[0].z;
    _max.x = corners[0].x; _max.y = corners[0].y; _max.z = corners[0].z;

    for (GLuint i = 1; i < 8; ++i)
    {
        _min.x = std::min(corners[i].x, _min.x);
        _min.y = std::min(corners[i].y, _min.y);
        _min.z = std::min(corners[i].z, _min.z);
        
        _max.x = std::max(corners[i].x, _max.x);
        _max.y = std::max(corners[i].y, _max.y);
        _max.z = std::max(corners[i].z, _max.z);
    }
}

GLF_NAMESPACE_END
