#version 330
			
layout(location = 0) out vec4 o_Color;

in vec3 normal;
in vec3 position;
in vec2 textCord;
in vec3 fragPos;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

uniform vec3 camPos;

uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;
uniform sampler2D specularTexture1;
uniform sampler2D specularTexture2;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	// DIFFUSE LIGHT
	vec3 unitNormal = normalize(normal);
	vec3 lighDirection = normalize(light.position - fragPos);
	float diffAmount = max(dot(unitNormal, lighDirection), 0.0);
	vec3 diffuse = light.diffuse * (diffAmount * material.diffuse);

	// SPECULAR LIGHT
	vec3 viewDirection = normalize(camPos - fragPos);
	vec3 reflexionDireciton = reflect(-lighDirection, unitNormal);
	float specAmount = pow(max(dot(viewDirection, reflexionDireciton), 0.0), material.shininess);
	vec3 specular = light.specular * (specAmount * material.specular);

	// OUTPUT
	vec3 result = ambient + diffuse + specular;
	//o_Color = mix(texture(diffuseTexture1, textCord), texture(diffuseTexture2, textCord), 0.2) * (lightColor, 1.0f) * (ambient + diffuse + specular);
	o_Color = vec4(result, 1.0) * mix(texture(diffuseTexture1, textCord), texture(diffuseTexture2, textCord), 0.2);
}