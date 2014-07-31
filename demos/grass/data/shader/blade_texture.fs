// -------------------------------------------------------------- 
// blade_color.fs 
// fragment shader of a grass blade in pure color (blademaker)
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#version 430 core

layout (location = 0, index = 0) out vec4 FragColor;

uniform sampler2D Texture;

in block
{
    vec4 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord; 
} In; 

float computeAO(float x)
{
    return 2.0 * x - x * x;
}

void main()
{
    vec2 tc = In.texcoord;
    tc.x = 0.25 * tc.x + (gl_FrontFacing? 0.0 : 0.25);
    FragColor = texture2D(Texture, tc) * clamp(computeAO(In.texcoord.y), 0.2, 1);
    //FragColor = texture2D(Texture, tc);
    FragColor.a = 1.0;
}



