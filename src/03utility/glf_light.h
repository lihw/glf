// -------------------------------------------------------------- 
// glf_light.h
// The data structure of real-time lights
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_LIGHT_H
#define GLF_LIGHT_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

enum LightTypeEnum
{
    LIGHT_NONE        = 0,
    LIGHT_POINT       = 1,
    LIGHT_DIRECTIONAL = 2,
    LIGHT_SPOT        = 3,

    LIGHT_TYPE_FIRST  = LIGHT_NONE,
    LIGHT_TYPE_LAST   = LIGHT_SPOT,
    LIGHT_TYPE_NUMBER = LIGHT_TYPE_LAST - LIGHT_TYPE_FIRST + 1,
};

struct AbstractLight
{
protected:
    AbstractLight(LightTypeEnum type);

public:
    LightTypeEnum  type;
    bool           visible;
    glm::vec3      position;
    glm::vec4      ambient;
    glm::vec4      diffuse;
    glm::vec4      specular;
    glm::vec3      attenuation;
    GLfloat        shininess;
};

struct PointLight : public AbstractLight
{
    PointLight();
};

struct DirectionalLight : public AbstractLight
{
    DirectionalLight();
};

struct SpotLight : public AbstractLight
{
    SpotLight();

    glm::vec3   spotDirection;
    GLfloat     spotCutoff;
    GLfloat     spotExponent;
};


GLF_NAMESPACE_END

#endif // !GLF_LIGHT_H
