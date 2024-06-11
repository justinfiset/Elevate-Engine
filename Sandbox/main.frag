#version 330 core
			
layout(location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TextCord;

uniform sampler2D ourTexture;

void main()
{
	//o_Color = v_Color;
	o_Color = texture(ourTexture, v_TextCord) * v_Color;
}	