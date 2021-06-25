#version 330 core
out vec4 fragColor;

layout (std140) uniform Camera
{
    mat4 uProjectionView;
    vec3 uViewPos;
};

struct Material {
    sampler2D ambient0;

    sampler2D diffuse0;
    sampler2D diffuse1;
    sampler2D diffuse2;

    sampler2D specular0;
    sampler2D specular1;
};

uniform Material uMaterial;

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

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;
in vec4 fragPosLightSpace;

uniform sampler2D uShadowMap;

float shadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5f + 0.5f;


    if (projCoords.z > 1.0f)
        return 0.0;

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

vec3 calculateDirLight(DirLight light, vec3 fragPos, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.direction);

    // ambient
    vec3 ambient = light.ambient * texture(uMaterial.ambient0, texCoord).rgb;

    // diffuse
    float diff = max(dot(-lightDir, normal), 0.0f);
    vec3 diffuse = light.diffuse * diff
                   * texture(uMaterial.diffuse0, texCoord).rgb;

    // specular
    // blinn-phong
    vec3 halfwayDir = normalize(viewDir - lightDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
    vec3 specular = light.specular * spec
                    * texture(uMaterial.specular0, texCoord).rgb;
    float shadow = shadowCalculation(fragPosLightSpace);
    return ambient + (1.0f - shadow) * (diffuse + specular);
}

void main() {
    vec3 viewDir = normalize(uViewPos - fragPos);
    vec3 result = calculateDirLight(uDirLight, fragPos, normal, viewDir);
    fragColor = vec4(result, 1.0);
}
