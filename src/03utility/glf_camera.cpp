// -------------------------------------------------------------- 
// camera.cpp
// Camera
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_camera.h"

#include "glf_light.h"
#include "math/glf_box.h"

#include <glm/gtc/matrix_inverse.hpp>

GLF_NAMESPACE_BEGIN

Camera::Camera()
{
    _modelview = glm::mat4(1.0f);
    _normal    = glm::mat3(1.0f);
    
    _projectionType = PERSPECTIVE;
    _znear          = 0.0001f;
    _zfar           = 1000.0f;
    _fov            = 45.0f;

    GLfloat aspect = 1.0f;
    _projection = glm::perspective(_fov, aspect, _znear, _zfar);
    _projectionModelview = _projection;
}

Camera::~Camera()
{
}

void Camera::setOrthogonal(GLfloat left, const GLfloat right, const GLfloat bottom, 
                           const GLfloat top, const GLfloat nearVal, const GLfloat farVal)
{
    _projectionType = ORTHOGONAL;
    _znear = nearVal;
    _zfar = farVal;
    _projection = glm::ortho(left, right, bottom, top, nearVal, farVal);
    _projectionModelview = _projection * _modelview;
}

void Camera::setPerspective(const GLfloat fovy, GLfloat aspect, const GLfloat nearVal, const GLfloat farVal)
{
    _projectionType = PERSPECTIVE;
    _znear = nearVal;
    _zfar = farVal;
    _projection = glm::perspective(fovy, aspect, nearVal, farVal);
    _projectionModelview = _projection * _modelview;
}

void Camera::lookAt(const GLfloat eyeX, const GLfloat eyeY, const GLfloat eyeZ, const GLfloat centerX, const GLfloat centerY, const GLfloat centerZ, const GLfloat upX, const GLfloat upY, const GLfloat upZ)
{
    _modelview = glm::lookAt(
            glm::vec3(eyeX, eyeY, eyeZ),
            glm::vec3(centerX, centerY, centerZ),
            glm::vec3(upX, upY, upZ));
    _projectionModelview = _projection * _modelview;
    _normal = glm::inverseTranspose(glm::mat3(_modelview));
}

void Camera::translate(const GLfloat x, const GLfloat y, const GLfloat z)
{
    _modelview[3][0] += x;
    _modelview[3][1] += y;
    _modelview[3][2] += z;
    _projectionModelview = _projection * _modelview;
    _normal = glm::inverseTranspose(glm::mat3(_modelview));
}

void Camera::rotate(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z)
{
    _modelview = glm::rotate(_modelview, angle, glm::vec3(x, y, z));
    _projectionModelview = _projection * _modelview;
    _normal = glm::inverseTranspose(glm::mat3(_modelview));
}

void Camera::identity()
{
    _modelview = glm::mat4(1.0f);
    _normal = glm::mat3(1.0f);
    _projectionModelview = _projection;
}
    

void Camera::setRotation(const GLfloat* rotation)
{
    _modelview[0][0] = rotation[0];
    _modelview[0][1] = rotation[1];
    _modelview[0][2] = rotation[2];

    _modelview[1][0] = rotation[3];
    _modelview[1][1] = rotation[4];
    _modelview[1][2] = rotation[5];

    _modelview[2][0] = rotation[6];
    _modelview[2][1] = rotation[7];
    _modelview[2][2] = rotation[8];
    
    _normal = glm::inverseTranspose(glm::mat3(_modelview));
    
    _projectionModelview = _projection * _modelview;
}

void Camera::setTranslation(const GLfloat* translation)
{
    _modelview[3][0] = translation[0];
    _modelview[3][1] = translation[1];
    _modelview[3][2] = translation[2];

    _projectionModelview = _projection * _modelview;
}

glm::vec3 Camera::getCameraPosition() const
{
    glm::mat4 test(1.0f);
    glm::mat4 result = glm::translate(test, glm::vec3(1.0f, 1.0f, 1.0f));

    glm::vec3 ret;
    ret.x = -glm::dot(_modelview[0], _modelview[3]);
    ret.y = -glm::dot(_modelview[1], _modelview[3]);
    ret.z = -glm::dot(_modelview[2], _modelview[3]);

    return ret;
}
    
void Camera::fromLight(const DirectionalLight &light, const Box& box)
{
    glm::vec3 center(0, 0, 0);
    glm::vec3 eye = -light.position * 100.0f;
    glm::vec3 up;
    glm::vec3 lightSource = -light.position;

    if (lightSource.y >= 0)
    {
        if (lightSource.y < 0.99f)
        {
            up = glm::vec3(0, 1, 0);
        }
        else
        {
            up = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        if (lightSource.y > -0.99f)
        {
            up = glm::vec3(0, -1, 0);
        }
        else
        {
            up = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
    }

    up = glm::normalize(up);

    // Generate the camera matrix
    lookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

    // Generate the projection matrix
    Box b(box);
    b.transform(_modelview);

    setOrthogonal(b.min().x, 
                  b.max().x, 
                  b.min().y, 
                  b.max().y, 
                  -b.max().z - 0.01f,
                  -b.min().z + 0.01f);
    
    //setOrthogonal(-10.0f, 10.0f, -10.0f, 10.0f, 0, 200.0f);
}

GLF_NAMESPACE_END
