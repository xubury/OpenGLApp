#version 330 core
out vec4 fragColor;

uniform sampler2D uShadowMap;

struct Material {
    sampler2D ambient0;

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

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;
in vec3 viewPos;
in vec4 fragPosLightSpace;

uniform PointLight uPointLight;

uniform Material uMaterial;

float shadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;


    if (projCoords.z > 1.0)
        return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

vec3 calculateDirLight(DirLight light, vec3 fragPos, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.direction);

    // ambient
    vec3 ambient = light.ambient * texture(uMaterial.ambient0, texCoord).rgb;

    // diffuse
    float diff = max(dot(normal, -lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff
                   * texture(uMaterial.diffuse0, texCoord).rgb;

    // specular
    // blinn-phong
    vec3 halfwayDir = normalize(viewDir - lightDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
    vec3 specular = light.specular * spec
                    * texture(uMaterial.specular0, texCoord).rgb;
    float shadow = shadowCalculation(fragPosLightSpace);
    return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(fragPos - light.position);

    // ambient
    vec3 ambient = light.ambient * texture(uMaterial.ambient0, texCoord).rgb;

    // diffuse
    float diff = max(dot(normal, -lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff
                   * texture(uMaterial.diffuse0, texCoord).rgb;

    // specular
    // blinn-phong
    vec3 halfwayDir = normalize(viewDir - lightDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
    vec3 specular = light.specular * spec
                    * texture(uMaterial.specular0, texCoord).rgb;

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
    vec3 result = calculateDirLight(uDirLight, fragPos, normal, viewDir);
    // result += calculatePointLight(uPointLight, fragPos, normal, viewDir);
    fragColor = vec4(result, 1.0);
}
