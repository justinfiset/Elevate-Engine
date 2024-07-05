#version 330
			
layout(location = 0) out vec4 o_Color;

in vec3 normal;
in vec2 textCord;
in vec3 fragPos;

// MATERIAL IMPL.
// TODO implement multiple diffuse texture functionallity
struct Material {
    sampler2D diffuse;
    sampler2D specular;
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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, textCord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, textCord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textCord));

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
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightVec = light.position - fragPos;
	float distance = length(lightVec);
    vec3 lightDir = normalize(lightVec);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, textCord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, textCord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textCord));
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