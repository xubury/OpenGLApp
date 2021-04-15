#version 330 core
out vec4 fragColor;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// fragPos and normal is in view space for convience.
// In this way, view direction equal -fragPos
in vec3 fragPos;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

in Light lightView;

uniform Material material;

void main() {
    // ambient
    vec3 ambient = lightView.ambient * texture(material.diffuse, texCoord).rgb;

    // diffuse
    vec3 lightDir = normalize(lightView.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightView.diffuse * diff 
                   * texture(material.diffuse, texCoord).rgb;

    // specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = lightView.specular * spec 
                    * texture(material.specular, texCoord).rgb;

    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, 1.0);
}
