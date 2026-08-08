layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 lightSpaceMatrix;

out vec3 normal;
out vec2 textCord;
out vec3 fragPos;
out vec4 fragPosLightSpace;
out mat3 TBN;

void main()
{
	fragPos = vec3(model * vec4(a_Position, 1.0f));
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0f);
	
	mat3 normalMatrix = mat3(transpose(inverse(model)));
    normal = normalMatrix * a_Normal; 
    textCord = a_TexCord;

	vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 B = normalize(normalMatrix * a_Bitangent);
    vec3 N = normalize(normalMatrix * a_Normal);
	TBN = mat3(T, B, N);

    gl_Position = viewProj * vec4(fragPos, 1.0);
}