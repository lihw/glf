// -------------------------------------------------------------- 
// phong.fs 
// fragment shader for phong shading
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

layout (location = 0, index = 0) out vec4 FragColor;

uniform sampler2D Texture;

in block
{
    vec3 position; // the pixel position in world space.
    vec3 normal;
    vec3 color;
    vec2 texcoord; 
} In; 

uniform vec3 CameraPosition;
uniform mat4 NormalMatrix;

struct PointLightStruct
{
    vec3 position; // the light position in world space.
    vec4 ambient;
    vec4 diffuse;
    vec4 specular; 
    float shinness;
};

uniform PointLightStruct PointLight;

vec4 phong(vec3 position, vec3 normal, PointLightStruct pointLight, vec4 grassColor)
{
    vec3 lightDir = normalize(pointLight.position - position.xyz); 
    vec3 eyeDir = normalize(CameraPosition.xyz - position.xyz);

    float diffuse = max(dot(normal, lightDir), 0);

    vec3 H = normalize(lightDir + eyeDir);
    float specular = pow(max(dot(H, normal), 0), pointLight.shinness); 

    vec4 color = pointLight.ambient + 
                 pointLight.diffuse * diffuse * vec4(grassColor.rgb, 1.0) +
                 pointLight.specular * specular;
        
    return color;
}

void main()
{
    vec4 grassColor = texture2D(Texture, In.texcoord); 
    vec4 normal = NormalMatrix * vec4(In.normal, 0);
    vec4 c = phong(In.position, normalize(normal.xyz), PointLight, grassColor);

    FragColor = vec4(c.xyz, 1.0);
}



