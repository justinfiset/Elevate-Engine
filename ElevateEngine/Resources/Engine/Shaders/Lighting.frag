precision highp float;
precision highp int;
precision highp sampler2DShadow;

in vec2 v_TexCoord;
out vec4 FragColor;

uniform sampler2D g_Color;
uniform sampler2D g_Normal;
uniform sampler2D g_Material;
uniform sampler2D g_Depth;
uniform sampler2D g_SSAO;

uniform vec3 camPos;
uniform mat4 inverseProjection;
uniform mat4 inverseView;
uniform mat4 view;

#define NUM_CASCADES 4
uniform sampler2DShadow shadowMapArray[NUM_CASCADES];
uniform mat4 lightSpaceMatrices[NUM_CASCADES];
uniform float cascadeSplitDepths[NUM_CASCADES];

struct DirLight {
    vec3 direction;
    vec3 color;
    float intensity;
};
uniform DirLight dirLight;

#define NR_POINT_LIGHTS 32
struct PointLight {
    vec3 position;
    float intensity;
    float constant;
    float linear;
    float quadratic;  
    vec3 color;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int u_NumPointLights;

#define NR_SPOT_LIGHTS 32
struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform int u_NumSpotLights;

vec3 ReconstructWorldPos(vec2 uv, float depth) {
    float z = depth * 2.0 - 1.0;
    vec4 clipSpacePos = vec4(uv * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePos = inverseProjection * clipSpacePos;
    viewSpacePos /= viewSpacePos.w;
    vec4 worldSpacePos = inverseView * viewSpacePos;
    return worldSpacePos.xyz;
}

vec3 CalcDirLight(DirLight light, vec3 albedo, float roughness, vec3 normal, vec3 viewDir, float ao, float shadowFactor)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    
    float shininess = max((2.0 / (roughness * roughness + 0.0001)) - 2.0, 1.0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), shininess);

    vec3 ambient  = light.color * albedo * ao;
    vec3 diffuse  = light.color * diff * albedo;
    vec3 specular = light.color * spec * (1.0 - roughness);

    return (ambient + (diffuse + specular) * shadowFactor) * light.intensity;
}

vec3 CalcPointLight(PointLight light, vec3 albedo, float roughness, vec3 normal, vec3 fragPos, vec3 viewDir, float ao)
{
    vec3 lightVec = light.position - fragPos;
    float distance = length(lightVec);
    vec3 lightDir = normalize(lightVec);

    float diff = max(dot(normal, lightDir), 0.0);
    float shininess = max((2.0 / (roughness * roughness + 0.0001)) - 2.0, 1.0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), shininess);

    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient  = light.color * light.intensity * albedo * ao;
    vec3 diffuse  = light.color * light.intensity * diff * albedo;
    vec3 specular = light.color * light.intensity * spec * (1.0 - roughness);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 albedo, float roughness, vec3 normal, vec3 fragPos, vec3 viewDir, float ao)
{
    vec3 lightVec = light.position - fragPos;
    float distance = length(lightVec);
    vec3 lightDir = normalize(lightVec);

    float diff = max(dot(normal, lightDir), 0.0);
    float shininess = max((2.0 / (roughness * roughness + 0.0001)) - 2.0, 1.0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), shininess);

    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float spotIntensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 ambient  = light.color * light.intensity * albedo * ao;
    vec3 diffuse  = light.color * light.intensity * diff * albedo;
    vec3 specular = light.color * light.intensity * spec * (1.0 - roughness);

    return (ambient + diffuse + specular) * attenuation * spotIntensity;
}

// Contournement WebGL pour les tableaux de samplers dynamiques via un switch statique
float SampleShadowMapCascade(int layer, vec3 sampleCoord)
{
    if (layer == 0) return texture(shadowMapArray[0], sampleCoord);
    if (layer == 1) return texture(shadowMapArray[1], sampleCoord);
    if (layer == 2) return texture(shadowMapArray[2], sampleCoord);
    return texture(shadowMapArray[3], sampleCoord);
}

vec2 GetShadowMapSize(int layer)
{
    if (layer == 0) return vec2(textureSize(shadowMapArray[0], 0));
    if (layer == 1) return vec2(textureSize(shadowMapArray[1], 0));
    if (layer == 2) return vec2(textureSize(shadowMapArray[2], 0));
    return vec2(textureSize(shadowMapArray[3], 0));
}

float CalcShadow(vec3 fragPos, vec3 localNormal)
{
    vec4 fragPosViewSpace = view * vec4(fragPos, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = NUM_CASCADES - 1;
    for (int i = 0; i < NUM_CASCADES; ++i)
    {
        if (depthValue < cascadeSplitDepths[i])
        {
            layer = i;
            break;
        }
    }

    vec3 lightDir = normalize(-dirLight.direction);
    float cosTheta = max(dot(normalize(localNormal), lightDir), 0.0);

    float normalOffsetScale = 0.05;
    vec3 biasedFragPos = fragPos + normalize(localNormal) * (normalOffsetScale * (1.0 - cosTheta));

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(biasedFragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
    {
        return 1.0; 
    }

    float depthBias = 0.000001 / (cascadeSplitDepths[layer] * 0.5);
    float compareDepth = projCoords.z - depthBias;
    
    vec2 texelSize = vec2(1.0) / GetShadowMapSize(layer);

    float shadowVisibility = 0.0;
    float totalWeight = 0.0;
    const int radius = 2;

    for (int ix = -radius; ix <= radius; ++ix)
    {
        for (int iy = -radius; iy <= radius; ++iy)
        {
            vec2 offset = vec2(float(ix), float(iy)) * texelSize;
            float sampleVis = SampleShadowMapCascade(layer, vec3(projCoords.xy + offset, compareDepth));
            float dist = length(vec2(float(ix), float(iy)));
            float weight = exp(-(dist * dist) / 4.0);

            shadowVisibility += sampleVis * weight;
            totalWeight += weight;
        }
    }

    return shadowVisibility / totalWeight;
}

void main()
{
    float depth = texture(g_Depth, v_TexCoord).r;
    if (depth >= 1.0)
    {
        discard;
    }

    vec3 rawAlbedo   = texture(g_Color, v_TexCoord).rgb;
    vec3 normal      = normalize(texture(g_Normal, v_TexCoord).rgb);
    vec4 material    = texture(g_Material, v_TexCoord);
    float roughness  = material.r;
    float metallic   = material.g;
    float materialAO = material.b;

    float ssaoFactor = texture(g_SSAO, v_TexCoord).r;
    float totalAO    = materialAO * ssaoFactor;

    vec3 fragPos = ReconstructWorldPos(v_TexCoord, depth);
    vec3 viewDir = normalize(camPos - fragPos);

    vec3 clampedAlbedo = min(rawAlbedo, vec3(1.0));

    float shadowFactor = CalcShadow(fragPos, normal);
    vec3 result = CalcDirLight(dirLight, clampedAlbedo, roughness, normal, viewDir, totalAO, shadowFactor);

    for (int i = 0; i < u_NumPointLights && i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], clampedAlbedo, roughness, normal, fragPos, viewDir, totalAO);    
    }

    for (int i = 0; i < u_NumSpotLights && i < NR_SPOT_LIGHTS; i++)
    {
        result += CalcSpotLight(spotLights[i], clampedAlbedo, roughness, normal, fragPos, viewDir, totalAO);    
    }

    if (max(rawAlbedo.r, max(rawAlbedo.g, rawAlbedo.b)) > 1.0)
    {
        result += rawAlbedo;
    }

    FragColor = vec4(result, 1.0);
}