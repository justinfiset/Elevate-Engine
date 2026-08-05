layout(location = 0) out vec4 o_Color;

in vec3 normal;
in vec2 textCord;
in vec3 fragPos;
in vec4 fragPosLightSpace;
 
vec3 defaultColor = vec3(0.8, 0.8, 0.8);
vec3 defaultAmbientColor = vec3(0.2, 0.2, 0.2);

uniform int u_NumPointLights;
uniform int u_NumSpotLights;

uniform sampler2D shadowMap;

// MATERIAL IMPL.
// TODO implement multiple diffuse texture functionallity
uniform sampler2D ambientTex;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform int has_ambientTex;

bool blinn = true; // use blinn-phong shading or not

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

uniform vec3 camPos;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};
uniform DirLight dirLight;

vec3 GetTextureColor(sampler2D tex, vec2 uv, vec3 defaultColor, int hasTexture) {
    if (hasTexture == 1) {
        return texture(tex, uv).rgb;
    }
    return defaultColor;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if(blinn) {
        vec3 halfDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }

    // combine results
    vec3 ambient  = light.ambient  * GetTextureColor(ambientTex, textCord, defaultAmbientColor, has_ambientTex);
    vec3 diffuse  = light.diffuse  * diff * GetTextureColor(diffuseTex, textCord, material.diffuse, 1);
    vec3 specular = light.specular * spec * GetTextureColor(specularTex, textCord, material.specular, 1);

    return (ambient + diffuse + specular) * light.intensity;
}

struct PointLight {    
    vec3 position;
    
    float intensity;
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightVec = light.position - fragPos;
	float distance = length(lightVec);
    vec3 lightDir = normalize(lightVec);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if(blinn) {
        vec3 halfDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }

    // attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient  * light.intensity * GetTextureColor(ambientTex, textCord, defaultAmbientColor, has_ambientTex);
    vec3 diffuse  = light.diffuse  * light.intensity * diff * GetTextureColor(diffuseTex, textCord, material.diffuse, 1);
    vec3 specular = light.specular * light.intensity * spec * GetTextureColor(specularTex, textCord, material.specular, 1);

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;

    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightVec = light.position - fragPos;
    float distance = length(lightVec);
    vec3 lightDir = normalize(lightVec);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.0;
    if (blinn)
    {
        vec3 halfDir = normalize(lightDir + viewDir);
        spec = pow(
            max(dot(normal, halfDir), 0.0),
            material.shininess
        );
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(
            max(dot(viewDir, reflectDir), 0.0),
            material.shininess
        );
    }

    float attenuation = 1.0 / (
        light.constant +
        light.linear * distance +
        light.quadratic * (distance * distance)
    );

    float theta = dot(
        lightDir,
        normalize(-light.direction)
    );

    float epsilon = light.innerCutoff - light.outerCutoff;

    float spotIntensity = clamp(
        (theta - light.outerCutoff) / epsilon,
        0.0,
        1.0
    );

    vec3 ambient = light.ambient * light.intensity 
        * GetTextureColor(ambientTex, textCord, defaultAmbientColor, has_ambientTex);
    vec3 diffuse = light.diffuse * light.intensity * diff
        * GetTextureColor(diffuseTex, textCord, material.diffuse, 1);
    vec3 specular = light.specular * light.intensity * spec
        * GetTextureColor(specularTex, textCord, material.specular, 1);

    ambient *= attenuation * spotIntensity;
    diffuse *= attenuation * spotIntensity;
    specular *= attenuation * spotIntensity;

    return ambient + diffuse + specular;
}

float CalcShadow()
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z > 1.0)
    {
        return 0.0;
    }

    vec3 lightDir = normalize(-dirLight.direction);

    float lightDot = max(dot(normal, lightDir), 0.0);

    float bias = max(
        0.02 * (1.0 - lightDot),
        0.002
    );

    float currentDepth = projCoords.z;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    float shadow = 0.0;
    float totalWeight = 0.0;

    // Rayon du filtre
    const int radius = 3;

    for (int x = -radius; x <= radius; ++x)
    {
        for (int y = -radius; y <= radius; ++y)
        {
            vec2 offset = vec2(x, y) * texelSize;

            float closestDepth =
                texture(shadowMap, projCoords.xy + offset).r;

            float difference =
                currentDepth - bias - closestDepth;

            // Transition douce autour de la frontière
            float sampleShadow = smoothstep(
                -0.001,
                 0.001,
                difference
            );

            // Poids gaussien approximatif
            float dist = length(vec2(x, y));

            float weight = exp(
                -(dist * dist) / 4.0
            );

            shadow += sampleShadow * weight;
            totalWeight += weight;
        }
    }

    return shadow / totalWeight;
}

void main()
{
	vec3 unitNormal = normalize(normal);
	vec3 viewDir = normalize(camPos - fragPos);

    // phase 1: Directional lighting    
    vec3 directionalLighting = CalcDirLight(dirLight, unitNormal, viewDir);
    vec3 result = directionalLighting * (1 - CalcShadow());

    // phase 2: Point lights
    for(int i = 0; i < u_NumPointLights && i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], unitNormal, fragPos, viewDir);    
    }

    // phase 3: Spot light
    for(int i = 0; i < u_NumSpotLights && i < NR_SPOT_LIGHTS; i++)
    {
        result += CalcSpotLight(spotLights[i], unitNormal, fragPos, viewDir);    
    }

    o_Color = vec4(result, 1.0);
}