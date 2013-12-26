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
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} Out;

out gl_PerVertex
{
    vec4 gl_Position;
};

uniform mat4 MVP;
uniform vec3 CameraPosition;
uniform float BladeWidth;
uniform float ThicknessThreshold;

#define WIDTH 1024.0
#define HEIGHT 768.0

void main()
{	
    // -------------------------------------------------------------- 
    // Expand the line in the binormal direction
    // -------------------------------------------------------------- 
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

    // The blade gets shaper at the tip.
    float w0 = (sqrt(1.0 - t[0] * t[0]) * 0.8 + 0.2) * BladeWidth;
    float w1 = (sqrt(1.0 - t[1] * t[1]) * 0.8 + 0.2) * BladeWidth;

    vec4 pWorld[] = 
    {
        vec4(gl_in[0].gl_Position.xyz - binormal[0] * w0, 1.0),
        vec4(gl_in[0].gl_Position.xyz + binormal[0] * w0, 1.0),
        vec4(gl_in[1].gl_Position.xyz - binormal[1] * w1, 1.0),
        vec4(gl_in[1].gl_Position.xyz + binormal[1] * w1, 1.0),
    };
    
    vec4 p[] = 
    {
        MVP * pWorld[0],
        MVP * pWorld[1],
        MVP * pWorld[2],
        MVP * pWorld[3],
    };
        
    vec3 color = vec3(1, 0, 0);

    // When the projected blade width is too small, we expand the blade orthogonally 
    // to the eye direction.
    vec2 proj[] = 
    {
        (p[0].xy / p[0].w) * vec2(WIDTH, HEIGHT),
        (p[1].xy / p[1].w) * vec2(WIDTH, HEIGHT),
        (p[2].xy / p[2].w) * vec2(WIDTH, HEIGHT),
        (p[3].xy / p[3].w) * vec2(WIDTH, HEIGHT),
    };

    //  p2 --- p3 
    //  |      |
    //  |      |
    //  p0 --- p1 
    //  
    vec2 e10 = proj[1] - proj[0];
    vec2 e20 = proj[2] - proj[0];
    
    // the thickness of the blade in window space
    float pe12 = dot(e10, normalize(e20));
    float thicknessSq = dot(e10, e10) - pe12 * pe12;

    if (thicknessSq < ThicknessThreshold)
    {
        vec3 eye[] =
        {
            normalize(CameraPosition.xyz - gl_in[0].gl_Position.xyz),
            normalize(CameraPosition.xyz - gl_in[1].gl_Position.xyz)
        };

        float eyeDistance0 = length(CameraPosition.xyz - gl_in[0].gl_Position.xyz);
        float eyeDistance1 = length(CameraPosition.xyz - gl_in[1].gl_Position.xyz);

        vec3 eyeExpansionDir[] = 
        {
            normalize(cross(In[0].Tangent, eye[0])),
            normalize(cross(In[1].Tangent, eye[1])),
        };

        pWorld[0] = vec4(gl_in[0].gl_Position.xyz - eyeExpansionDir[0] * 0.001 * eyeDistance0, 1.0);
        pWorld[1] = vec4(gl_in[0].gl_Position.xyz + eyeExpansionDir[0] * 0.001 * eyeDistance0, 1.0);
        pWorld[2] = vec4(gl_in[1].gl_Position.xyz - eyeExpansionDir[1] * 0.001 * eyeDistance1, 1.0);
        pWorld[3] = vec4(gl_in[1].gl_Position.xyz + eyeExpansionDir[1] * 0.001 * eyeDistance1, 1.0);

        p[0] = MVP * pWorld[0];
        p[1] = MVP * pWorld[1];
        p[2] = MVP * pWorld[2];
        p[3] = MVP * pWorld[3];

        color = vec3(0, 1, 0);
    }

    // -------------------------------------------------------------- 
    // Expand the line to a grass blade 
    // -------------------------------------------------------------- 
    gl_Position = p[0];
    Out.normal = In[0].Normal;
    Out.position = pWorld[0];
    Out.color = color;
    Out.texcoord = vec2((1.0f - w0 / BladeWidth) * 0.5, t[0]);
    EmitVertex();
    
    gl_Position = p[1];
    Out.position = pWorld[1];
    Out.normal = In[0].Normal;
    Out.color = color;
    Out.texcoord = vec2((1.0f + w0 / BladeWidth) * 0.5, t[0]);
    EmitVertex();
    
    gl_Position = p[2];
    Out.position = pWorld[2];
    Out.normal = In[1].Normal;
    Out.color = color;
    Out.texcoord = vec2((1.0f - w1 / BladeWidth) * 0.5, t[1]);
    EmitVertex();
    
    gl_Position = p[3];
    Out.position = pWorld[3];
    Out.normal = In[1].Normal;
    //Out.color = In[1].Color;
    //Out.color = vec3(length(d[1]) / 4, 0, 0);
    Out.texcoord = vec2((1.0f + w1 / BladeWidth) * 0.5, t[1]);
    Out.color = color;
    EmitVertex();

    EndPrimitive();
}

