// -------------------------------------------------------------- 
// blade_point.gs
// render the points of the blade knots
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#version 430 core

layout(lines, invocations = 1) in;
layout(points, max_vertices = 2) out;

in block
{
    vec4 color;
} In[];

layout(stream = 0) out block
{
    vec4 color;
} Out;

out gl_PerVertex
{
    vec4 gl_Position;
};

uniform mat4 MVP;

void main()
{	
    gl_Position = MVP * gl_in[0].gl_Position;
    Out.color = In[0].color;
    EmitVertex();
    EndPrimitive();
    
    gl_Position = MVP * gl_in[1].gl_Position;
    Out.color = In[1].color;
    EmitVertex();
    EndPrimitive();
}
