layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 model;
uniform mat4 viewProj;

out vec3 normal;
out vec2 textCord;

out vec3 fragPos;

void main()
{
	fragPos = vec3(model * vec4(a_Position, 1.0f));
	normal = mat3(transpose(inverse(model))) * a_Normal; 
	textCord = a_TexCord;

    gl_Position = viewProj * vec4(fragPos, 1.0);
}