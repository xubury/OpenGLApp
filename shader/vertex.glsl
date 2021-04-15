#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

out vec3 fragPos;
out vec3 color;
out vec2 texCoord;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightWorldPos;
out vec3 lightPos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    fragPos = vec3(view * model * vec4(aPos, 1.0));
    color = aColor;
    texCoord = aTexCoord;
    normal = normalize(mat3(transpose(inverse(view * model))) * aNormal);

    lightPos = vec3(view * vec4(lightWorldPos, 1.0));
}

