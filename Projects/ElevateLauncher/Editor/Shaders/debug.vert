#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 viewProj;

out vec4 o_Color;

void main()
{
    gl_Position = viewProj * vec4(a_Position, 1.0);
    o_Color = a_Color;
}