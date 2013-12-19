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

layout (location = 0, index = 0) out vec4 FragColor;

in block
{
    vec4 color; 
} In; 

void main()
{
    FragColor = In.color;
}



