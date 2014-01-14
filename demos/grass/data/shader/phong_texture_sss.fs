// -------------------------------------------------------------- 
// phong_texture_sss.fs 
// fragment shader for phong shading plus texture
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

layout (location = 0, index = 0) out vec4 FragColor;

uniform sampler2D Texture;

#define PI 3.1415926
#define KERNEL_SIZE 3


in block
{
    vec3 position; // the pixel position in world space.
    vec3 normal;
    vec3 color;
    vec2 texcoord; 
} In; 

uniform vec3 CameraPosition;
uniform mat4 NormalMatrix;
uniform mat4 LightMVP;
uniform float Scale;
uniform sampler2D ShadowMap;

struct LightStruct
{
    vec3 direction; // in world space.
    vec4 ambient;
    vec4 diffuse;
    vec4 specular; 
    float shinness;
};

uniform LightStruct Light;

vec3 T(float s)
{
    return vec3(0.233, 0.455, 0.649) * exp(-s * s / 0.0064) +
           vec3(0.100, 0.336, 0.344) * exp(-s * s / 0.0484) +
           vec3(0.118, 0.198, 0.000) * exp(-s * s / 0.1870) +
           vec3(0.113, 0.007, 0.007) * exp(-s * s / 0.5670) +
           vec3(0.358, 0.004, 0.000) * exp(-s * s / 1.9900) +
           vec3(0.078, 0.000, 0.000) * exp(-s * s / 7.4100);
}

vec3 reflectance(vec3 position, vec3 normal, LightStruct light, vec4 albedo)
{
    vec3 lightDir = -normalize(light.direction.xyz); // inverse the light direction
    vec3 eyeDir   = normalize(CameraPosition.xyz - position.xyz);

    float diffuse = max(dot(normal, lightDir), 0);

    vec3 H = normalize(lightDir + eyeDir);
    float specular = pow(max(dot(H, normal), 0), light.shinness); 

    vec4 color = light.ambient + 
                 light.diffuse * diffuse * albedo +
                 light.specular * specular;
        
    return color.rgb;
}

vec3 transmittance(vec3 position, vec3 normal, LightStruct light, vec4 albedo)
{
    // compute the light travel distance before hitting the current pixel
    vec4 shadowPosition = LightMVP * vec4(position, 1.0);
    shadowPosition.xyz /= shadowPosition.w;
    vec2 texcoord = (shadowPosition.xy + vec2(1, 1)) * 0.5;

    // PCF shadow
    vec2 smSize = vec2(textureSize(ShadowMap, 0));
    
    float totalWeight = 0;
    float totalDepth = 0;
    for (int dx = (1 - KERNEL_SIZE) / 2; dx <= KERNEL_SIZE / 2; dx++) 
    { 
        for (int dy = (1 - KERNEL_SIZE) / 2; dy <= KERNEL_SIZE / 2; dy++) 
        { 
            float size = 2.4;
            // standard deviation, when kernel/2 > 3*sigma, it's close to zero, here we use 1.5 instead
            float sigma = (KERNEL_SIZE / 2.0) / size; 
            float e = -1 * (dx * dx + dy * dy)/ (2 * sigma * sigma);
            float weight = 1 / (2 * PI * sigma * sigma) * exp(e);

            vec2 tc = texcoord + vec2(float(dx) / smSize.x, float(dy) / smSize.y);
            float depth = texture(ShadowMap, tc).x;

            totalDepth += depth * weight;
            totalWeight += weight;
        }
    }
    
    float d1 = totalDepth / totalWeight;
    float d2 = shadowPosition.z;
    float dist = abs(d1 - d2);
    
    vec3 lightDir = -normalize(light.direction.xyz); // inverse the light direction
    float s = Scale * dist;
    float E = max(0.3 + dot(normal, lightDir), 0.0);

    vec3 ret = T(s) * E * light.diffuse.rgb * albedo.rgb;
    
    return ret * 0.001 + vec3(d1, d1, d1);
}

void main()
{
    vec4 normal = NormalMatrix * vec4(In.normal, 0);
    normal.xyz = normalize(normal.xyz);

    vec4 albedo = texture2D(Texture, In.texcoord);

    vec3 refl = reflectance(In.position, normal.xyz, Light, albedo);
    vec3 trans = transmittance(In.position, -normal.xyz, Light, albedo);

    FragColor.rgb = refl * 0.001 + trans;
    FragColor.a = 1.0f;
}


