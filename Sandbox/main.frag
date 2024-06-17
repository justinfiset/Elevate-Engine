#version 330 core
			
layout(location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TextCord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//o_Color = v_Color;
	//o_Color = texture(texture2, v_TextCord);
	o_Color = mix(texture(texture1, v_TextCord), texture(texture2, v_TextCord), 0.2);
}