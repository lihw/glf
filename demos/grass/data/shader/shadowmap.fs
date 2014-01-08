// -------------------------------------------------------------- 
// shadowmap.fs 
// fragment shader for generating shadow map
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
    vec3 position; // the pixel position in world space.
    vec3 normal;
    vec3 color;
    vec2 texcoord; 
} In; 

in vec4 gl_FragCoord;

void main()
{
    FragColor = vec4(gl_FragCoord.zzz, 1.0);
}



