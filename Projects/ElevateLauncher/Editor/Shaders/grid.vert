#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCord;

uniform mat4 model;
uniform mat4 viewProj;

out vec2 fragPos2D;

void main()
{
	vec3 fragPos = vec3(model * vec4(a_Position, 1.0f));
	fragPos2D = fragPos.xz;

    gl_Position = viewProj * vec4(fragPos, 1.0);
}