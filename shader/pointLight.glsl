#version 330 core
out vec4 fragColor;

in vec2 texCoord;

layout (std140) uniform Camera
{
    mat4 uProjectionView;
    vec3 uViewPos;
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

uniform PointLight uPointLight;

uniform sampler2D uGBufferPosition;
uniform sampler2D uGBufferNormal;
uniform sampler2D uGBufferAlbedoSpec;

vec3 calculatePointLight(PointLight light, vec3 fragPos, vec3 normal,
                         vec3 viewDir, vec3 ambient, vec3 diffuse, vec3 specular) {
    vec3 lightDir = normalize(fragPos - light.position);

    // ambient
    ambient = light.ambient * ambient;

    // diffuse
    float diff = max(dot(-lightDir, normal), 0.0f);
    diffuse = light.diffuse * diff * diffuse;

    // specular
    // blinn-phong
    vec3 halfwayDir = normalize(viewDir - lightDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0f);
    specular = light.specular * spec * specular;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance
                              + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //spotlight
    float theta = dot(lightDir, normalize(light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

void main() {
    vec3 fragPos = texture(uGBufferPosition, texCoord).rgb;
    vec3 normal = texture(uGBufferNormal, texCoord).rgb;
    vec3 diffuse = texture(uGBufferAlbedoSpec, texCoord).rgb;
    vec3 specular = vec3(texture(uGBufferAlbedoSpec, texCoord).a);
    vec3 viewDir = normalize(uViewPos - fragPos);

    vec3 lighting = vec3(0.f);
    lighting += calculatePointLight(uPointLight, fragPos, normal,
                                    viewDir, diffuse, diffuse, specular);
    fragColor = vec4(lighting, 1.0f);
}
