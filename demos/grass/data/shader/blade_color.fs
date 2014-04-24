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

in block
{
    vec4 color; 
} In; 

void main()
{
    FragColor = In.color;
}



