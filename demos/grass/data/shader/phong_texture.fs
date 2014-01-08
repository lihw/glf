// -------------------------------------------------------------- 
// phong_texture.fs 
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


in block
{
    vec3 position; // the pixel position in world space.
    vec3 normal;
    vec3 color;
    vec2 texcoord; 
} In; 

uniform vec3 CameraPosition;
uniform mat4 NormalMatrix;

struct LightStruct
{
    vec3 direction; // in world space.
    vec4 ambient;
    vec4 diffuse;
    vec4 specular; 
    float shinness;
};

uniform LightStruct Light;

vec4 phong(vec3 position, vec3 normal, LightStruct light)
{
    vec3 lightDir = -normalize(light.direction); // inverse the light direction
    vec3 eyeDir   = normalize(CameraPosition.xyz - position.xyz);

    float diffuse = max(dot(normal, lightDir), 0);

    vec3 H = normalize(lightDir + eyeDir);
    float specular = pow(max(dot(H, normal), 0), light.shinness); 

    vec4 color = light.ambient + 
                 light.diffuse * diffuse +
                 light.specular * specular;
        
    return color;
}

void main()
{
    vec4 normal = NormalMatrix * vec4(In.normal, 0);
    vec4 c = phong(In.position, normalize(normal.xyz), Light);
    vec4 grassColor = texture2D(Texture, In.texcoord) * c.x; 

    FragColor = vec4(grassColor.rgb, 1.0);
}


