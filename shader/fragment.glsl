#version 330 core
out vec4 fragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


in vec3 fragPos;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
in Light lightViewSpace;

uniform Material material;

uniform sampler2D texture0;

void main() {
    // ambient
    vec3 ambient = lightViewSpace.ambient * material.ambient;

    // diffuse
    vec3 lightDir = normalize(lightViewSpace.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightViewSpace.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = lightViewSpace.specular * (spec * material.specular); 

    vec3 light = ambient + diffuse + specular;

    fragColor = texture(texture0, texCoord) * vec4(light, 1.0);
}

