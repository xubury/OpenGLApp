#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 fragPos;
out vec2 texCoord;
out vec3 normal;
out vec4 fragPosLightSpace;

layout (std140) uniform Camera
{
    mat4 uProjectionView;
    vec3 uViewPos;
};

uniform mat4 uModel;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (std140) uniform Light
{
    mat4 uLightSpaceMatrix;
    DirLight uDirLight;
};


void main() {
    gl_Position = uProjectionView * uModel * vec4(aPos, 1.0);

    fragPos = vec3(uModel * vec4(aPos, 1.0));
    texCoord = aTexCoord;
    normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
    fragPosLightSpace = uLightSpaceMatrix * vec4(fragPos, 1.0);
}
