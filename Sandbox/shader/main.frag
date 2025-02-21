layout(location = 0) out vec4 o_Color;

in vec3 normal;
in vec2 textCord;
in vec3 fragPos;

vec3 defaultColor = vec3(0.8, 0.8, 0.8);

// MATERIAL IMPL.
// TODO implement multiple diffuse texture functionallity
uniform sampler2D ambientTex;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

uniform bool blinn;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

uniform vec3 camPos;

// DIFFUSE LIGHT CALCULATION
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

vec3 GetTextureColor(sampler2D tex, vec2 uv, vec3 defaultColor) {
    // TODO:change the way to process cuz texture can't include black;
    vec3 texColor = texture(tex, uv).rgb;
    if (texColor == vec3(0.0, 0.0, 0.0)) { 
        return defaultColor;
    } else {
        return texColor;
    }
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
    vec3 ambient  = light.ambient  * GetTextureColor(ambientTex, textCord, material.ambient);
    vec3 diffuse  = light.diffuse  * diff * GetTextureColor(diffuseTex, textCord, material.diffuse);
    vec3 specular = light.specular * spec * GetTextureColor(specularTex, textCord, material.specular);

    return (ambient + diffuse + specular);
}

// POINT LIGHT CALCULATION
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// todo change number of lights depending on the real number in the scene
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
    vec3 ambient  = light.ambient  * GetTextureColor(ambientTex, textCord, material.ambient);
    vec3 diffuse  = light.diffuse  * diff * GetTextureColor(diffuseTex, textCord, material.diffuse);
    vec3 specular = light.specular * spec * GetTextureColor(specularTex, textCord, material.specular);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main()
{
	// TODO optimize calculations : reflect, diffuse + sepcular + texture sampling
	vec3 unitNormal = normalize(normal);
	vec3 viewDir = normalize(camPos - fragPos);

    // phase 1: Directional lighting    
    vec3 result = CalcDirLight(dirLight, unitNormal, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], unitNormal, fragPos, viewDir);    
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
	// OUTPUT
    o_Color = vec4(result, 1.0);
}