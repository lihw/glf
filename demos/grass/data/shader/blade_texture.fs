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
    vec3 normal;
    vec3 color;
    vec2 texcoord; 
} In; 

void main()
{
    FragColor = texture2D(Texture, In.texcoord);
    FragColor.a = 1.0;
}



