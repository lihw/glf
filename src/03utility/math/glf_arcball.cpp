// -------------------------------------------------------------- 
// glf_arcball.cpp
// To rotate the scene based on the mouse input in the screen.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#include "glf_arcball.h"

#include <glm/gtx/quaternion.hpp>

#include <stdio.h>
#include <string.h>

GLF_NAMESPACE_BEGIN

Arcball::Arcball()
{
    _moving = GL_FALSE;
}

void Arcball::restart()
{
    _moving = GL_FALSE;

    _lastRotation = _rotation;
}

void Arcball::updateMouse(const GLfloat x, const GLfloat y)
{
    if (!_moving)
    {
        mapToSphere(x, y, &_lastArcballCoordinate[0]);
        _moving = GL_TRUE;
    }
    else
    {
        mapToSphere(x, y, &_arcballCoordinate[0]);

        glm::vec3 perp = glm::cross(_lastArcballCoordinate, _arcballCoordinate);

        // Compute the rotating quaternion
        GLfloat l = glm::length(perp);

        glm::quat quat;

        if (l > 1e-4f) 
        {
            quat[0] = perp[0];
            quat[1] = perp[1];
            quat[2] = perp[2];
            quat[3] = glm::dot(_lastArcballCoordinate, _arcballCoordinate); 
        } 
        else 
        {
            quat[0] = quat[1] = quat[2] = quat[3] = 0.0f;
        }

        // Accumulate the rotation.
        _rotation = quat * _lastRotation;
        
        _rotationMatrix = glm::toMat3(_rotation);
    }
}

const GLfloat* Arcball::getRotationMatrixF() const
{
    return &_rotationMatrix[0][0];
}

const glm::mat3& Arcball::getRotationMatrix() const
{
    return _rotationMatrix;
}

void Arcball::mapToSphere(const GLfloat x, const GLfloat y, GLfloat* coordinate)
{
    coordinate[0] = (GLfloat)x;
    coordinate[1] = (GLfloat)y;

    GLfloat l = coordinate[0] * coordinate[0] + coordinate[1] * coordinate[1];

    if (l > 1.0f) 
    {
        GLfloat inv = 1.0f / sqrt(l);
        coordinate[0] *= inv;
        coordinate[1] *= inv;
        coordinate[2] = 0.0f;
    } 
    else 
    {
        coordinate[2] = sqrt(1.0f - l);
    }
}

GLF_NAMESPACE_END
