#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D texture0;

void main() {
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor;

    float ambientStrengh = 0.1;
    vec3 ambient = ambientStrengh * lightColor;

    vec3 light = diffuse + ambient;

    fragColor = texture(texture0, texCoord) * vec4(light, 1.0);
}

