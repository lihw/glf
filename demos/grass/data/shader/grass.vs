// -------------------------------------------------------------- 
// grass.vs
// vertex shader of grass blade
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#version 430 core

#define VERTEX_ATTRIB_POSITION   0
#define VERTEX_ATTRIB_NORMAL     1
#define VERTEX_ATTRIB_TEXCOORD   2
#define VERTEX_ATTRIB_COLOR      3
#define VERTEX_ATTRIB_UNAMED0    4
#define VERTEX_ATTRIB_UNAMED1    5
#define VERTEX_ATTRIB_UNAMED2    6
#define VERTEX_ATTRIB_UNAMED3    7
#define VERTEX_ATTRIB_UNAMED4    8

layout (location = VERTEX_ATTRIB_POSITION) in vec3 Position;
layout (location = VERTEX_ATTRIB_NORMAL)   in vec3 Normal;
layout (location = VERTEX_ATTRIB_COLOR)    in vec3 Color;
layout (location = VERTEX_ATTRIB_UNAMED0)  in float Distance;
layout (location = VERTEX_ATTRIB_UNAMED1)  in vec3 Tangent;

out gl_PerVertex
{
    vec4 gl_Position;
};

out block
{
    vec3 Normal; 
    vec3 Tangent;
    vec3 Color; 
    float Distance;
} Out;

void main()
{
    // FIXME: Get the positions and normals from the computation shader.
    gl_Position = vec4(Position, 1);

    Out.Normal = Normal;
    Out.Color  = Color;
    Out.Distance = Distance;
    Out.Tangent = Tangent;
}


