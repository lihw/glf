// -------------------------------------------------------------- 
// ao.fs 
// fragment shader of computing ambient occlusion.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#version 420 core

layout(binding = 0, r32ui) coherent uniform uimage2D StartOffsetBufferX;
layout(binding = 1, rgba32ui) coherent uniform uimage2D GBufferX1;
layout(binding = 2, rgba32ui) coherent uniform uimage2D GBufferX2;

layout(binding = 3, r32ui) coherent uniform uimage2D StartOffsetBufferY;
layout(binding = 4, rgba32ui) coherent uniform uimage2D GBufferY1;
layout(binding = 5, rgba32ui) coherent uniform uimage2D GBufferY2;

uniform mat4 MVPX; 
uniform mat4 MVPY;

layout (location = 0, index = 0) out vec4 FragColor;

in block
{
    vec3 position; // the pixel position in world space.
    vec3 normal;
    vec3 color;
    vec2 texcoord; 
} In; 

uniform vec2 InvViewport;

// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter14.html
float computeShadow(vec3 neighborPosition, vec3 neighborNormal,
                    vec3 position, vec3 normal)
{
    vec3 v = neighborPosition - position;
    float rSq = dot(v, v);

    return (1.0 - inversesqrt(1.0 / rSq + 1)) * 
        clamp(0, 1, dot(neighborNormal, v)) * 
        clamp(0, 1, 4.0 * dot(normal, -v));
}

ivec2 getImageCoord(uint index, uvec2 size)
{
    uvec2 ret = uvec2(index % size.x, index / size.x);
    return ivec2(ret);
}

void main()
{
    float shadow = 1;

    // -------------------------------------------------------------- 
    // X direciton.
    // -------------------------------------------------------------- 

    vec4 pos;
    vec2 texcoord;
    uint nodeIndex;

    // Transform the the pixel's world position to the pixel location in the start offset buffer.
    pos = MVPX * vec4(In.position, 1.0);
    texcoord = pos.xy / pos.w * 0.5 + vec2(0.5); 
    // For those out of the frustum, we put them to the closest grid cell.
    texcoord = clamp(texcoord * vec2(imageSize(StartOffsetBufferX)), vec2(0), 
        imageSize(StartOffsetBufferX) - vec2(1));
    nodeIndex = imageLoad(StartOffsetBufferX, ivec2(texcoord)).x;

    if (nodeIndex != 0xffffffff)
    {
        shadow = 0.5;
    }

    //while (nodeIndex != 0xffffffff)
    //{
    //    uvec4 p = imageLoad(GBufferX1, getImageCoord(nodeIndex, imageSize(GBufferX1)));
    //    uvec4 n = imageLoad(GBufferX2, getImageCoord(nodeIndex, imageSize(GBufferX2)));

    //    shadow += computeShadow(In.position, In.normal, 
    //                            vec3(floatBitsToUint(p.x),
    //                                 floatBitsToUint(p.y),
    //                                 floatBitsToUint(p.z)),
    //                            vec3(floatBitsToUint(n.x),
    //                                 floatBitsToUint(n.y),
    //                                 floatBitsToUint(n.z))),

    //   nodeIndex = p.w;
    //}

    // -------------------------------------------------------------- 
    // Y direciton.
    // -------------------------------------------------------------- 
    //pos = MVPY * vec4(In.position, 1.0);
    //texcoord = pos.xy / pos.w * vec2(imageSize(StartOffsetBufferY));
    //nodeIndex = imageLoad(StartOffsetBufferY, ivec2(texcoord)).x;

    //while (nodeIndex != 0xffffffff)
    //{
    //    uvec4 p = imageLoad(GBufferY1, getImageCoord(nodeIndex, imageSize(GBufferY1)));
    //    uvec4 n = imageLoad(GBufferY2, getImageCoord(nodeIndex, imageSize(GBufferY2)));

    //    shadow += computeShadow(In.position, In.normal, 
    //                            vec3(floatBitsToUint(p.x),
    //                                 floatBitsToUint(p.y),
    //                                 floatBitsToUint(p.z)),
    //                            vec3(floatBitsToUint(n.x),
    //                                 floatBitsToUint(n.y),
    //                                 floatBitsToUint(n.z))),

    //   nodeIndex = p.w;
    //}

    // -------------------------------------------------------------- 
    // Final color
    // -------------------------------------------------------------- 
    FragColor = vec4(shadow, shadow, shadow, 1.0);
}

