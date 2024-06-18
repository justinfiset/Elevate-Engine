#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCord;

uniform mat4 model;
uniform mat4 viewProj;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TextCord;

void main()
{
    gl_Position = viewProj * model * vec4(a_Position, 1.0);
	v_Position = a_Position;
	v_Color = a_Color;
	v_TextCord = a_TexCord;
}