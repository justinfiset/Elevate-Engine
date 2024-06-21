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

uniform sampler2D texture1;
uniform sampler2D texture2;

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


	o_Color = mix(texture(texture1, textCord), texture(texture2, textCord), 0.2) * (lightColor, 1.0f) * (ambiant + diffuse + specular);
}