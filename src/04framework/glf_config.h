// -------------------------------------------------------------- 
// glf_config.h
// The configuration of the window
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_CONFIG_H
#define GLF_CONFIG_H

struct GLFConfig
{
    int windowWidth;
    int windowHeight;
    int redBits;
    int greenBits;
    int blueBits;
    int alphaBits;
    int depthBits;
    int stencilBits;
    int multisamples;

    GLFConfig()
    {
        windowWidth = 640;
        windowHeight = 480;

        redBits = greenBits = blueBits = alphaBits = 8;
        depthBits = 24;
        stencilBits = 8;

        multisamples = 1;
    };
};

#endif // !GLF_CONFIG_H
