#version 330 core
out vec4 fragColor;

in vec2 texCoord;

layout (std140) uniform Camera
{
    mat4 uProjectionView;
    vec3 uViewPos;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (std140) uniform Light
{
    mat4 uLightSpaceMatrix;
    DirLight uDirLight;
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
uniform sampler2D uGBufferAmbient;

uniform sampler2D uShadowMap;

float shadowCalculation(vec3 fragPos, vec3 lightDir, vec3 normal)
{
    vec4 fragPosLightSpace = uLightSpaceMatrix * vec4(fragPos, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5f + 0.5f;


    if (projCoords.z > 1.0f)
        return 0.0f;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005f;
    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(uShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }
    shadow /= 9.0f;

    return shadow;
}

vec3 calculateDirLight(DirLight light, vec3 fragPos, vec3 normal, vec3 viewDir,
                       vec3 ambient, vec3 diffuse, vec3 specular) {
    vec3 lightDir = normalize(light.direction);

    // ambient
    ambient = light.ambient * ambient;

    // diffuse
    float diff = max(dot(-lightDir, normal), 0.0f);
    diffuse = light.diffuse * diff * diffuse;

    // specular
    // blinn-phong
    vec3 halfwayDir = normalize(viewDir - lightDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
    specular = light.specular * spec * specular;

    float shadow = shadowCalculation(fragPos, lightDir, normal);
    return ambient + (1.0f - shadow) * (diffuse + specular);
}

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
    vec3 ambient = texture(uGBufferAmbient, texCoord).rgb;
    vec3 viewDir = normalize(uViewPos - fragPos);

    vec3 lighting = vec3(0.f);
    lighting = calculateDirLight(uDirLight, fragPos, normal,
                                  viewDir, ambient, diffuse, specular);
    lighting += calculatePointLight(uPointLight, fragPos, normal,
                                   viewDir, ambient, diffuse, specular);
    fragColor = vec4(lighting, 1.0f);
}
