#version 330
			
layout(location = 0) out vec4 o_Color;

in vec3 normal;
in vec3 position;
in vec3 color;
in vec2 textCord;
in vec3 fragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;
uniform sampler2D specularTexture1;
uniform sampler2D specularTexture2;

void main()
{
	float ambiant = 0.2;
	// DIFFUSE LIGHT
	vec3 unitNormal = normalize(normal);
	vec3 lighDirection = normalize(lightPos - fragPos);

	float diffuse = max(dot(unitNormal, lighDirection), 0.0f);

	// SPECULAR LIGHT
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - fragPos);
	vec3 reflexionDireciton = reflect(-lighDirection, unitNormal);
	float specAmount = pow(max(dot(viewDirection, reflexionDireciton), 0.0f), 8);
	float specular = specAmount * specularLight;

	
	//o_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	o_Color = mix(texture(diffuseTexture1, textCord), texture(diffuseTexture2, textCord), 0.2) * (lightColor, 1.0f) * (ambiant + diffuse + specular);
}