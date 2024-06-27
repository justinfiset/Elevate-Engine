#version 330
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCord;

uniform mat4 model;
uniform mat4 viewProj;

out vec3 normal;
out vec3 position;
out vec3 color;
out vec2 textCord;

out vec3 fragPos;

void main()
{
    gl_Position = viewProj * model * vec4(a_Position, 1.0);
	position = a_Position;
	color = vec3(1.0f, 1.0f, 1.0f);
	textCord = a_TexCord;
	normal = mat3(transpose(inverse(model))) * a_Normal; 
	fragPos = vec3(model * vec4(a_Position, 1.0f));
}