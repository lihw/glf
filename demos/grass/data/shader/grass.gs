// -------------------------------------------------------------- 
// grass.gs
// Geometry expansion of a grass blade
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#version 430 core

layout(lines, invocations = 1) in;
layout(triangle_strip, max_vertices = 4) out;

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
uniform float BladeWidth;

void main()
{	
    vec3 binormal[] = 
    {
        cross(In[0].Normal, In[0].Tangent),
        cross(In[1].Normal, In[1].Tangent)
    };
    
    float t[] = 
    {
        In[0].Distance,
        In[1].Distance,
    };

    float w0 = (sqrt(1.0 - t[0] * t[0]) * 0.8 + 0.2) * BladeWidth; 
    float w1 = (sqrt(1.0 - t[1] * t[1]) * 0.8 + 0.2) * BladeWidth;

    // -------------------------------------------------------------- 
    // Expand the line to a grass blade 
    // -------------------------------------------------------------- 
    gl_Position = MVP * (vec4(gl_in[0].gl_Position.xyz - binormal[0] * w0, 1.0));
    Out.normal = In[0].Normal;
    Out.color = In[0].Color;
    EmitVertex();
    
    gl_Position = MVP * (vec4(gl_in[0].gl_Position.xyz + binormal[0] * w0, 1.0));
    Out.normal = In[0].Normal;
    Out.color = In[0].Color;
    EmitVertex();
    
    gl_Position = MVP * (vec4(gl_in[1].gl_Position.xyz - binormal[1] * w1, 1.0));
    Out.normal = In[1].Normal;
    Out.color = In[1].Color;
    EmitVertex();
    
    gl_Position = MVP * (vec4(gl_in[1].gl_Position.xyz + binormal[1] * w1, 1.0));
    Out.normal = In[1].Normal;
    Out.color = In[1].Color;
    EmitVertex();

    EndPrimitive();
}

