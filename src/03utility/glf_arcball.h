// -------------------------------------------------------------- 
// glf_arcball.h
// To rotate the scene based on the mouse input in the screen.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_ARCBALL_H
#define GLF_ARCBALL_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

class Arcball
{
public:
    Arcball();

    // Restart the arcball movement. Clean the current state.
    void restart();

    // Update the current mouse position in the viewport coordinate (OpenGL coordinate).
    void updateMouse(const GLfloat x, const GLfloat y);

    // Fetch the current 3x3 rotation matrix of this arcball.
    const GLfloat* getRotationMatrixF() const;

    const glm::mat3& getRotationMatrix() const;

private:
    glm::quat _lastRotation;
    glm::quat _rotation;
    glm::mat3 _rotationMatrix;
    glm::vec3 _lastArcballCoordinate;
    glm::vec3 _arcballCoordinate;
    GLboolean _moving; // In moving mode, the rotation matrix keeps updating.

    // Map the viewport coordinate mouse position to the sphere coordinate.
    void mapToSphere(const GLfloat x, const GLfloat y, GLfloat* coordinate);
};

GLF_NAMESPACE_END

#endif
