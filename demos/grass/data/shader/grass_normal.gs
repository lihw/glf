// -------------------------------------------------------------- 
// grass_normal.gs
// display normal of the grass blade
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#version 430 core

layout(lines, invocations = 1) in;
layout(line_strip, max_vertices = 2) out;

in block
{
    vec3 Normal;
    vec3 Tangent;
    vec3 Color;
    float Distance;
} In[];

layout(stream = 0) out block
{
    vec3 normal;
    vec3 color;
} Out;

out gl_PerVertex
{
    vec4 gl_Position;
};

uniform mat4 MVP;

void main()
{	
    vec3 binormal[] = 
    {
        cross(In[0].Normal, In[0].Tangent),
        cross(In[1].Normal, In[1].Tangent)
    };


    // -------------------------------------------------------------- 
    // Expand the line to a grass blade 
    // -------------------------------------------------------------- 
    gl_Position = MVP * gl_in[0].gl_Position;
    Out.normal = In[0].Normal;
    Out.color = In[0].Color;
    EmitVertex();
    
    gl_Position = MVP * (vec4(gl_in[0].gl_Position.xyz + In[0].Normal * 0.2, 1.0));
    Out.normal = In[0].Normal;
    Out.color = In[0].Color;
    EmitVertex();

    EndPrimitive();
    
    gl_Position = MVP * gl_in[1].gl_Position;
    Out.normal = In[1].Normal;
    Out.color = In[1].Color;
    EmitVertex();
    
    gl_Position = MVP * (vec4(gl_in[1].gl_Position.xyz + In[1].Normal * 0.2, 1.0));
    Out.normal = In[1].Normal;
    Out.color = In[1].Color;
    EmitVertex();

    EndPrimitive();
}

