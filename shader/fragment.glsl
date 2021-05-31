#version 330 core
out vec4 fragColor;

struct Material {
    sampler2D diffuse0;
    sampler2D diffuse1;
    sampler2D diffuse2;

    sampler2D specular0;
    sampler2D specular1;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 direction;
    
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;
in vec3 viewPos;

uniform PointLight pointLight;
uniform DirLight dirLight;

uniform Material material;

vec3 calculateDirLight(DirLight light, vec3 fragPos, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.direction);

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse0, texCoord).rgb;
        
    // diffuse
    float diff = max(dot(normal, -lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff 
                   * texture(material.diffuse0, texCoord).rgb;

    // specular
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = light.specular * spec 
                    * texture(material.specular0, texCoord).rgb;
    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(fragPos - light.position);

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse0, texCoord).rgb;
        
    // diffuse
    float diff = max(dot(normal, -lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff 
                   * texture(material.diffuse0, texCoord).rgb;

    // specular
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = light.specular * spec 
                    * texture(material.specular0, texCoord).rgb;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance
                              + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //spotlight
    float theta = dot(lightDir, normalize(light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

void main() {
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = calculateDirLight(dirLight, fragPos, normal, viewDir);
    result += calculatePointLight(pointLight, fragPos, normal, viewDir);
    fragColor = vec4(result, 1.0);
}
