#version 330 core
			
layout(location = 0) out vec4 o_Color;
in vec3 o_Position;

void main()
{
	//o_Color = vec4(0.8, 0.2, 0.3, 1.0);
	o_Color = vec4(o_Position.xyz, 1.0);
}	