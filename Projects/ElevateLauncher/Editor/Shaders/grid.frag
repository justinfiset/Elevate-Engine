#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 fragPos2D;

// Colors
uniform vec4 lineColor;
uniform vec4 backgroundColor;

float lineWidth = 0.005;
vec2 cellSize = vec2(1);

vec4 displayColor;  

void main()
{
    vec2 cellCoord = mod(fragPos2D, cellSize);
    vec2 cutoff = vec2(1.0 - lineWidth, 1.0 - lineWidth);

    vec2 alpha = step(cutoff, cellCoord);

    if (max(alpha.x, alpha.y) == 0.0)
        displayColor = backgroundColor;
	else displayColor = lineColor;
    o_Color = displayColor;
}