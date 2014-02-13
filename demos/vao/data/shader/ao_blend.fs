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

// -------------------------------------------------------------- 
// in
// -------------------------------------------------------------- 

in vec4 gl_FragCoord;

#define KERNEL_SIZE 3

// -------------------------------------------------------------- 
// out
// -------------------------------------------------------------- 
layout (location = 0, index = 0) out vec4 FragColor;


void main()
{
    float density = 0.0;

    uint nodeIndex;

    // For those out of the frustum, we put them to the closest grid cell.
    
    ivec2 tc = ivec2(gl_FragCoord.xy);
    nodeIndex = imageLoad(StartOffsetBuffer, tc).x;
        
    uvec2 size = imageSize(FragmentBuffer);

    while (nodeIndex != 0xffffffff)
    {
        ivec2 c1 = ivec2(nodeIndex % size.x, nodeIndex / size.x);
        ivec2 c2 = ivec2((nodeIndex + 1) % size.x, (nodeIndex + 1) / size.x);

        uvec4 p = imageLoad(FragmentBuffer, c1);
        uvec4 n = imageLoad(FragmentBuffer, c2);

        density += 0.2;

        nodeIndex = p.w;
    }
    
    FragColor = vec4(density, density, density, 1.0);// * 0.01 + vec4(1, 0, 0, 1);
}



