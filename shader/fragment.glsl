#version 330 core
out vec4 fragColor;

struct Material {
    sampler2D ambient0;

    sampler2D diffuse0;
    sampler2D diffuse1;
    sampler2D diffuse2;

    sampler2D specular0;
    sampler2D specular1;
};

uniform Material uMaterial;
in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;

void main() {
    fragColor = vec4(0);
}
