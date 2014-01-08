// -------------------------------------------------------------- 
// glf_camera.h
// Camera
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_CAMERA_H
#define GLF_CAMERA_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN


class DirectionalLight;


class Camera 
{
public:
    // By default, the projection camera a perspective camera with default
    // values while the modelview matrix is an identity matrix.
    Camera();

    ~Camera();

    // Create a perspective camera.
    void setPerspective(const GLfloat fovy, GLfloat aspect, const GLfloat nearVal, const GLfloat farVal);

    // Create an orthogonal camera.
    void setOrthogonal(GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top, 
        const GLfloat nearVal, const GLfloat farVal);

    // Create a modelview camera.
    void lookAt(const GLfloat eyeX, const GLfloat eyeY, const GLfloat eyeZ, const GLfloat centerX, 
            const GLfloat centerY, const GLfloat centerZ, const GLfloat upX, const GLfloat upY, const GLfloat upZ);

    // Update the left top 3x3 matrix of the modelview matrix. The input 'rotation' is a 3x3 matrix in column order.
    void setRotation(const GLfloat* rotation);

    // Update the first three elements in the first column of the modelview matrix.
    void setTranslation(const GLfloat* translation);

    // Translate a camera with an offset (only affect the modelview matrix)
    void translate(const GLfloat x, const GLfloat y, const GLfloat z);

    // Rotate the camera with certain axis by some angle in radians.(only affect the modelview matrix)
    void rotate(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z); 

    // Restore the camera position and orientation to default.
    void identity();

    // Create a camera from a directional light; used in shadow map.
    void fromLight(const DirectionalLight &light);

    // Get the positon of the camera
    glm::vec3 getCameraPosition() const;
    
    // Get combined projection* modelview camera matrix
    const glm::mat4& getProjectionModelviewMatrix() const
    {
        return _projectionModelview;
    }

    // Get the modelview matrix only.
    const glm::mat4& getModelviewMatrix() const
    { 
        return _modelview;
    }

    // Get the projection matrix only.
    const glm::mat4& getProjectionMatrix() const
    {
        return _projection;
    }

    // Get the normal transformation matrix only.
    const glm::mat3& getNormalMatrix() const
    {
        return _normal;
    }

    GLfloat getZNear() const
    {
        return _znear;
    }

    GLfloat getZFar() const
    {
        return _zfar;
    }

private:
    GLint          _projectionType;
    glm::mat4      _modelview;
    glm::mat4      _projection;
    glm::mat4      _projectionModelview;
    glm::mat3      _normal;
    GLfloat        _fov;
    GLfloat        _znear;
    GLfloat        _zfar;

    enum 
    {
        ORTHOGONAL,
        PERSPECTIVE,
    };
};


GLF_NAMESPACE_END



#endif // !GLF_CAMERA_H

