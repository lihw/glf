// -------------------------------------------------------------- 
// grass_color.fs
// fragment shader of a grass blade
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
    vec3 normal; 
    vec3 color; 
} In; 

//uniform vec4 Color;

in vec4 gl_FragCoord;

void main()
{
    FragColor = vec4(In.color, 1); 
}


