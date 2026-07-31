layout(location = 0) out vec4 o_Color;

in vec2 fragPos2D;

// Colors
uniform vec4 lineColor;
uniform vec4 backgroundColor;

const vec4 xAxisColor = vec4(1.0, 0.2, 0.2, 1.0); // Red
const vec4 zAxisColor = vec4(0.2, 0.4, 1.0, 1.0); // Blue

float lineWidth = 0.005;
float axisWidth = 0.015;
vec2 cellSize = vec2(1.0);

vec4 displayColor;  

void main()
{
    bool isXAxis = abs(fragPos2D.y) < axisWidth;
    bool isZAxis = abs(fragPos2D.x) < axisWidth;

    if (isXAxis)
    {
        displayColor = xAxisColor;
    }
    else if (isZAxis)
    {
        displayColor = zAxisColor;
    }
    else
    {
        vec2 cellCoord = mod(fragPos2D, cellSize);
        vec2 cutoff = vec2(1.0 - lineWidth, 1.0 - lineWidth);

        vec2 alpha = step(cutoff, cellCoord);

        if (max(alpha.x, alpha.y) == 0.0)
        {
            displayColor = backgroundColor;
        }
        else
        {
            displayColor = lineColor;
        }
    }

    o_Color = displayColor;
}