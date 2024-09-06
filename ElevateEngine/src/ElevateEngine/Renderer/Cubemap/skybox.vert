R""(#version 330 core
layout (location = 0) in vec3 a_Pos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = a_Pos;
    gl_Position = projection * view * vec4(a_Pos, 1.0);
})""