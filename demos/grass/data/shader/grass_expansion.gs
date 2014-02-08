// -------------------------------------------------------------- 
// grass_expansion.gs
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
layout(triangle_strip, max_vertices = 24) out;

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

struct Vertex
{
    vec3 position; // world position
    vec4 projection; // mvp * world position
    vec2 texcoord;
    vec3 normal;
};

uniform mat4 MVP;
uniform vec3 CameraPosition;
uniform float BladeWidth;
uniform float ThicknessThreshold;

void createFace(Vertex v[8], int i0, int i1, int i2, int i3)
{
    gl_Position  = v[i0].projection;
    Out.position = v[i0].position;
    Out.normal   = v[i0].normal;
    Out.color    = vec3(0, 0, 0);
    Out.texcoord = v[i0].texcoord;
    EmitVertex();
    
    gl_Position  = v[i1].projection;
    Out.position = v[i1].position;
    Out.normal   = v[i1].normal;
    Out.color    = vec3(0, 0, 0);
    Out.texcoord = v[i1].texcoord;
    EmitVertex();
    
    gl_Position  = v[i2].projection;
    Out.position = v[i2].position;
    Out.normal   = v[i2].normal;
    Out.color    = vec3(0, 0, 0);
    Out.texcoord = v[i2].texcoord;
    EmitVertex();

    gl_Position  = v[i3].projection;
    Out.position = v[i3].position;
    Out.normal   = v[i3].normal;
    Out.color    = vec3(0, 0, 0);
    Out.texcoord = v[i3].texcoord;
    EmitVertex();

    EndPrimitive();
}

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

    //  p2 --- p3 
    //  |      |
    //  |      |
    //  p0 --- p1 
    vec4 pWorld[] = 
    {
        vec4(gl_in[0].gl_Position.xyz - binormal[0] * w0, 1.0),
        vec4(gl_in[0].gl_Position.xyz + binormal[0] * w0, 1.0),
        vec4(gl_in[1].gl_Position.xyz - binormal[1] * w1, 1.0),
        vec4(gl_in[1].gl_Position.xyz + binormal[1] * w1, 1.0),
    };


    // Texture coordinate
    vec2 texcoord[] = 
    {
        vec2((1.0f - w0 / BladeWidth) * 0.5, t[0]),
        vec2((1.0f + w0 / BladeWidth) * 0.5, t[0]),
        vec2((1.0f - w1 / BladeWidth) * 0.5, t[1]),
        vec2((1.0f + w1 / BladeWidth) * 0.5, t[1]),
    };

    // Expand the quad to a hexahedron to the orthogonal direction to the eye direction.
    vec3 eyeRay[] =
    {
        normalize(CameraPosition.xyz - pWorld[0].xyz),
        normalize(CameraPosition.xyz - pWorld[1].xyz),
        normalize(CameraPosition.xyz - pWorld[2].xyz),
        normalize(CameraPosition.xyz - pWorld[3].xyz)
    };

    vec3 expansionDir[] = 
    {
        normalize(cross(In[0].Tangent, eyeRay[0])),
        normalize(cross(In[0].Tangent, eyeRay[1])),
        normalize(cross(In[1].Tangent, eyeRay[2])),
        normalize(cross(In[1].Tangent, eyeRay[3])),
    };

    // Generate the 8 vertices of the hexahedron

    // p0-> v0 v1
    // p1-> v2 v3
    // p2-> v4 v5
    // p3-> v6 v7
    //

    Vertex v[8];

    v[0].position = pWorld[0].xyz - expansionDir[0] * ThicknessThreshold * 0.1;
    v[1].position = pWorld[0].xyz + expansionDir[0] * ThicknessThreshold * 0.1;
    v[2].position = pWorld[1].xyz - expansionDir[1] * ThicknessThreshold * 0.1;
    v[3].position = pWorld[1].xyz + expansionDir[1] * ThicknessThreshold * 0.1;
    v[4].position = pWorld[2].xyz - expansionDir[2] * ThicknessThreshold * 0.1;
    v[5].position = pWorld[2].xyz + expansionDir[2] * ThicknessThreshold * 0.1;
    v[6].position = pWorld[3].xyz - expansionDir[3] * ThicknessThreshold * 0.1;
    v[7].position = pWorld[3].xyz + expansionDir[3] * ThicknessThreshold * 0.1;
    
    v[0].projection = MVP * vec4(v[0].position, 1.0);
    v[1].projection = MVP * vec4(v[1].position, 1.0);
    v[2].projection = MVP * vec4(v[2].position, 1.0);
    v[3].projection = MVP * vec4(v[3].position, 1.0);
    v[4].projection = MVP * vec4(v[4].position, 1.0);
    v[5].projection = MVP * vec4(v[5].position, 1.0);
    v[6].projection = MVP * vec4(v[6].position, 1.0);
    v[7].projection = MVP * vec4(v[7].position, 1.0);

    v[0].texcoord = texcoord[0];
    v[1].texcoord = texcoord[0];
    v[2].texcoord = texcoord[1];
    v[3].texcoord = texcoord[1];
    v[4].texcoord = texcoord[2];
    v[5].texcoord = texcoord[2];
    v[6].texcoord = texcoord[3];
    v[7].texcoord = texcoord[3];

    v[0].normal = -In[0].Normal;
    v[1].normal = In[0].Normal;
    v[2].normal = -In[0].Normal;
    v[3].normal = In[0].Normal;
    v[4].normal = -In[1].Normal;
    v[5].normal = In[1].Normal;
    v[6].normal = -In[1].Normal;
    v[7].normal = In[1].Normal;

    // Create the topology of the hexahedron
    createFace(v, 1, 3, 5, 7); // front
    createFace(v, 2, 0, 6, 4); // back
    createFace(v, 0, 1, 4, 5); // left
    createFace(v, 3, 2, 7, 6); // right
    createFace(v, 3, 1, 2, 0); // bottom
    createFace(v, 5, 7, 4, 6); // top
}

