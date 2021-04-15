#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

uniform vec3 lightColor;
in vec3 lightPos;

uniform sampler2D texture0;

void main() {
    float ambientStrengh = 0.1;
    vec3 ambient = ambientStrengh * lightColor;

    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularStrengh = 1.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
    vec3 sepecular = specularStrengh * spec * lightColor; 

    vec3 light = ambient + diffuse + sepecular;

    fragColor = texture(texture0, texCoord) * vec4(light, 1.0);
}

