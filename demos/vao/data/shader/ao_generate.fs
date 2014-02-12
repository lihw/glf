// -------------------------------------------------------------- 
// ao_genenerate.fs 
// fragment shader of recording all fragments into certain cell
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
in block
{
    vec3 position; // the pixel position in world space.
    vec3 normal;
} In; 

in vec4 gl_FragCoord;

// -------------------------------------------------------------- 
// out
// -------------------------------------------------------------- 
layout (location = 0, index = 0) out vec4 FragColor;

void storeFragment(vec2 coord, vec3 position, vec3 normal)
{
    uint index = atomicCounterIncrement(Atomic);
    
    // We need to add two elements at one time.
    index = index * 2;
    // Update the start offset at this pixel location.
    uint previousNode = imageAtomicExchange(StartOffsetBuffer, ivec2(coord), index);
    
    uvec2 size = imageSize(StartOffsetBuffer);
    // The next storage position.
    uvec2 p = uvec2(index % size.x, index / size.x);

    // Write position and normal to fragment buffer
    imageStore(FragmentBuffer, ivec2(p), 
            uvec4(floatBitsToUint(position.x), 
                  floatBitsToUint(position.y), 
                  floatBitsToUint(position.z), 
                  previousNode));
    imageStore(FragmentBuffer, ivec2(p.x + 1, p.y), 
            uvec4(floatBitsToUint(normal.x), 
                  floatBitsToUint(normal.y), 
                  floatBitsToUint(normal.z), 
                  previousNode));
}

void main()
{
    storeFragment(gl_FragCoord.xy, In.position, normalize(In.normal)); 
    FragColor = vec4(1, 0, 0, 1);
}



