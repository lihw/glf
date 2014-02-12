// -------------------------------------------------------------- 
// ao_blend.fs
// fragment shader of blending all fragments in the same coordinate
// into a pixel.
//
// deep ambient occlusion map
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#version 420 core

// -------------------------------------------------------------- 
// constant
// -------------------------------------------------------------- 
layout(binding = 0, offset = 0) uniform atomic_uint Atomic;
layout(binding = 1, r32ui) coherent uniform uimage2D StartOffsetBuffer;
layout(binding = 2, rgba32ui) coherent uniform uimage2D FragmentBuffer; 

uniform mat4 AOMVP;

// -------------------------------------------------------------- 
// in
// -------------------------------------------------------------- 
in block
{
    vec3 position; // the pixel position in world space.
    vec3 normal;
} In; 

in vec4 gl_FragCoord;

#define KERNEL_SIZE 3

// -------------------------------------------------------------- 
// out
// -------------------------------------------------------------- 
layout (location = 0, index = 0) out vec4 FragColor;


void main()
{
    float density = 0.0;

    vec4 pos;
    vec2 texcoord;
    uint nodeIndex;

    // Transform the the pixel's world position to the pixel location in the start offset buffer.
    pos = AOMVP * vec4(In.position, 1.0);
    texcoord = pos.xy / pos.w * 0.5 + vec2(0.5, 0.5); 
    // For those out of the frustum, we put them to the closest grid cell.
    texcoord = texcoord * vec2(imageSize(StartOffsetBuffer));
    
    for (int dx = (1 - KERNEL_SIZE) / 2; dx <= KERNEL_SIZE / 2; dx++) 
    { 
        for (int dy = (1 - KERNEL_SIZE) / 2; dy <= KERNEL_SIZE / 2; dy++) 
        { 
            ivec2 tc = ivec2(texcoord) + ivec2(dx, dy);
            nodeIndex = imageLoad(StartOffsetBuffer, tc).x;

            while (nodeIndex != 0xffffffff)
            {
                uvec2 size = imageSize(StartOffsetBuffer);
                ivec2 c = ivec2(nodeIndex % size.x, nodeIndex / size.x);
                
                uvec4 p = imageLoad(FragmentBuffer, c);
                uvec4 n = imageLoad(FragmentBuffer, ivec2(c.x + 1, c.y));

                density += 0.1f;

                nodeIndex = p.w;
            }
        }
    }
    
    FragColor = vec4(density, density, density, 1.0);
}



