#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 fragPos;
out vec2 texCoord;
out vec3 normal;
out vec3 viewPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    fragPos = vec3(model * vec4(aPos, 1.0));
    texCoord = aTexCoord;
    normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    viewPos = vec3(-view[3]);
}

