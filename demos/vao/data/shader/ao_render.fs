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

#version 430 core

// -------------------------------------------------------------- 
// constant
// -------------------------------------------------------------- 
layout(binding = 1, r32ui) coherent uniform uimage2D StartOffsetBuffer;
layout(binding = 2, rgba32ui) coherent uniform uimage2D FragmentBuffer; 

uniform mat4 AOMVP;

layout (location = 0, index = 0) out vec4 FragColor;

in block
{
    vec3 position; // the pixel position in world space.
    vec3 normal;
} In; 

#define KERNEL_SIZE 3

// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter14.html
float computeShadow(vec3 neighborPosition, vec3 neighborNormal,
                    vec3 position, vec3 normal)
{
    vec3 v = neighborPosition - position;
    float rSq = dot(v, v);

    v = normalize(v);

    //return (1 - inversesqrt(1.0 / rSq + 1)) * 
    //    clamp(0, 1, dot(normalize(neighborNormal), -v)) * 
    //    clamp(0, 1, 4.0 * dot(normalize(normal), v));
    //return  
    //     (1 - sqrt(1.0 / (rSq + 1))) * 
    //    clamp(0, 1, dot(normalize(neighborNormal), -v)) * 
    //    clamp(0, 1, 4.0 * dot(normalize(normal), v));

    return 0.5 * step(rSq, 0.063) * clamp(0, 1, dot(normal, v));
}

void main()
{
    float shadow = 1;

    vec4 pos;
    vec2 texcoord;
    uint nodeIndex;

    // Transform the the pixel's world position to the pixel location in the start offset buffer.
    pos = AOMVP * vec4(In.position, 1.0);
    texcoord = pos.xy / pos.w * 0.5 + vec2(0.5, 0.5); 
    vec2 c = texcoord;
    // For those out of the frustum, we put them to the closest grid cell.
    texcoord = texcoord * vec2(imageSize(StartOffsetBuffer));
    
    uvec2 size = imageSize(FragmentBuffer);

    vec3 normal = normalize(In.normal);

    for (int dx = (1 - KERNEL_SIZE) / 2; dx <= KERNEL_SIZE / 2; dx++) 
    { 
        for (int dy = (1 - KERNEL_SIZE) / 2; dy <= KERNEL_SIZE / 2; dy++) 
        { 
            ivec2 tc = ivec2(texcoord) + ivec2(dx, dy);
            nodeIndex = imageLoad(StartOffsetBuffer, tc).x;

            while (nodeIndex != 0xffffffff)
            {
                ivec2 c1 = ivec2(nodeIndex % size.x, nodeIndex / size.x);
                ivec2 c2 = ivec2((nodeIndex + 1) % size.x, (nodeIndex + 1) / size.x);

                uvec4 p = imageLoad(FragmentBuffer, c1);
                uvec4 n = imageLoad(FragmentBuffer, c2);

                shadow -= computeShadow(In.position, normal, 
                                        vec3(uintBitsToFloat(p.x),
                                             uintBitsToFloat(p.y),
                                             uintBitsToFloat(p.z)),
                                        vec3(uintBitsToFloat(n.x),
                                             uintBitsToFloat(n.y),
                                             uintBitsToFloat(n.z))),
                //shadow -= 0.01;
                nodeIndex = p.w;
            }
        }
    }

    // -------------------------------------------------------------- 
    // Final color
    // -------------------------------------------------------------- 
    //color = normalize(In.normal) * 0.5 + vec3(0.5);
    FragColor = vec4(shadow, shadow, shadow, 1.0);// * 0.001 + vec4(color, 1);
    //FragColor = vec4(c, 0.0, 1.0);// * 0.001 + vec4(color, 1);
}

    //if (nodeIndex == 0xffffffff)
    //{
    //    color = vec3(0, 0, 1);
    //}
    //else
    //{
    //    uvec4 p = imageLoad(GBufferX1, getImageCoord(nodeIndex, imageSize(GBufferX1)));
    //    uvec4 n = imageLoad(GBufferX2, getImageCoord(nodeIndex, imageSize(GBufferX2)));

    //    //color = normalize(vec3(uintBitsToFloat(n.x),
    //    //                       uintBitsToFloat(n.y),
    //    //                       uintBitsToFloat(n.z))) * 0.5 + vec3(0.5);
    //    color = vec3(0, 1, 0);
    //    if (p.w == 0xffffffff)
    //    {
    //        color = vec3(1, 1, 0);
    //    }

    //}
