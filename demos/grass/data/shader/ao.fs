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

layout (binding = 6) uniform sampler2D Texture;

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
    //vec3 v = neighborPosition - position;
    //float rSq = dot(v, v);

    //v = normalize(v);

    //return (1 - inversesqrt(1.0 / rSq + 1)) * 
    //    clamp(0, 1, dot(normalize(neighborNormal), -v)) * 
    //    clamp(0, 1, 4.0 * dot(normalize(normal), v));

    return 0.05;
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
    texcoord = pos.xy / pos.w * 0.5 + vec2(0.5, 0.5); 
    vec3 color = texture2D(Texture, texcoord).rgb;
    // For those out of the frustum, we put them to the closest grid cell.
    texcoord = texcoord * vec2(imageSize(StartOffsetBufferX));
    nodeIndex = imageLoad(StartOffsetBufferX, ivec2(texcoord)).x;

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

    while (nodeIndex != 0xffffffff)
    {
        uvec4 p = imageLoad(GBufferX1, getImageCoord(nodeIndex, imageSize(GBufferX1)));
        uvec4 n = imageLoad(GBufferX2, getImageCoord(nodeIndex, imageSize(GBufferX2)));

        shadow -= computeShadow(In.position, normalize(In.normal), 
                                vec3(uintBitsToFloat(p.x),
                                     uintBitsToFloat(p.y),
                                     uintBitsToFloat(p.z)),
                                vec3(uintBitsToFloat(n.x),
                                     uintBitsToFloat(n.y),
                                     uintBitsToFloat(n.z))),

        nodeIndex = p.w;
    }

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
    //                            vec3(UintBitsToFloat(p.x),
    //                                 UintBitsToFloat(p.y),
    //                                 UintBitsToFloat(p.z)),
    //                            vec3(UintBitsToFloat(n.x),
    //                                 UintBitsToFloat(n.y),
    //                                 UintBitsToFloat(n.z))),

    //   nodeIndex = p.w;
    //}

    // -------------------------------------------------------------- 
    // Final color
    // -------------------------------------------------------------- 
    //color = normalize(In.normal) * 0.5 + vec3(0.5);
    FragColor = vec4(shadow, shadow, shadow, 1.0);// * 0.001 + vec4(color, 1);
}

