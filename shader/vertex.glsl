#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 fragPos;
out vec2 texCoord;
out vec3 normal;
out vec3 viewPos;
out vec4 fragPosLightSpace;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uLightSpaceMatrix;


void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

    fragPos = vec3(uModel * vec4(aPos, 1.0));
    texCoord = aTexCoord;
    normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
    viewPos = vec3(-uView[3]);
    fragPosLightSpace = uLightSpaceMatrix * vec4(fragPos, 1.0);
}

