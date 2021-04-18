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

struct Light {
    vec3 position;
    vec4 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;
out Light lightView;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    fragPos = vec3(view * model * vec4(aPos, 1.0));
    color = aColor;
    texCoord = aTexCoord;
    normal = normalize(mat3(transpose(inverse(view * model))) * aNormal);

    lightView = light;
    lightView.position = vec3(view * vec4(light.position, 1.0));
    lightView.direction = view * light.direction;
}

