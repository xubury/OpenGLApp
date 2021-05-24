#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

