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

struct Light {
    vec3 position;
    vec4 direction;
    
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
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
    bool directional = false;
    if (lightView.direction.w < 1e-6) // directional light
        directional = true;

    vec3 lightDir;
    if (directional)
        lightDir = normalize(vec3(lightView.direction));
    else 
        lightDir = normalize(fragPos - lightView.position);

    // ambient
    vec3 ambient = lightView.ambient * texture(material.diffuse0, texCoord).rgb;
        
    // diffuse
    float diff = max(dot(normal, -lightDir), 0.0);
    vec3 diffuse = lightView.diffuse * diff 
                   * texture(material.diffuse0, texCoord).rgb;

    // specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = lightView.specular * spec 
                    * texture(material.specular0, texCoord).rgb;

    if (!directional) {
        float distance = length(lightView.position - fragPos);
        float attenuation = 1.0 / (lightView.constant + lightView.linear * distance
                                  + lightView.quadratic * (distance * distance));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    //spotlight
    float theta = dot(lightDir, normalize(vec3(lightView.direction)));
    float epsilon = lightView.cutOff - lightView.outerCutOff;
    float intensity = clamp((theta - lightView.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0f);
}
