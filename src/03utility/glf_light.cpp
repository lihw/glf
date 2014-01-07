// -------------------------------------------------------------- 
// glf_light.cpp
// The data structure of real-time lights
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_light.h"

GLF_NAMESPACE_BEGIN

AbstractLight::AbstractLight(LightTypeEnum type)
    : type(type)
{
    visible     = true;
    ambient     = glm::vec4(0);
    diffuse     = glm::vec4(0);
    specular    = glm::vec4(0);
    attenuation = glm::vec3(0);
    shininess   = 1.0f;
}

PointLight::PointLight()
    : AbstractLight(LIGHT_POINT)
{
}

DirectionalLight::DirectionalLight()
    : AbstractLight(LIGHT_DIRECTIONAL)
{
}

SpotLight::SpotLight()
    : AbstractLight(LIGHT_SPOT)
{
    spotDirection    = glm::vec3(0, 0, -1); 
    spotCutoff       = 45.0f;
    spotExponent     = 0.0f;
}

GLF_NAMESPACE_END
