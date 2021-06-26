#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aBoneIds;
layout (location = 4) in vec4 aWeights;

out vec3 fragPos;
out vec2 texCoord;
out vec3 normal;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 uBoneTransform[MAX_BONES];

layout (std140) uniform Camera
{
    mat4 uProjectionView;
    vec3 uViewPos;
};

uniform mat4 uModel;

void main() {
    vec3 totalPosition = vec3(0.f);
    bool hasBoneAnim = false;
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if(aBoneIds[i] == -1)
            continue;
        if(aBoneIds[i] >= MAX_BONES) {
            totalPosition = aPos;
            break;
        }
        vec4 localPosition = uBoneTransform[aBoneIds[i]] * vec4(aPos, 1.0f);
        totalPosition += localPosition.xyz * aWeights[i];
        vec3 localNormal = mat3(uBoneTransform[aBoneIds[i]]) * aNormal;
        hasBoneAnim = true;
    }

    if (hasBoneAnim) {
        fragPos = (uModel * vec4(totalPosition, 1.0f)).xyz;
    } else {
        fragPos = (uModel * vec4(aPos, 1.0f)).xyz;
    }

    texCoord = aTexCoord;
    normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);

    gl_Position = uProjectionView * vec4(fragPos, 1.0f);
}
