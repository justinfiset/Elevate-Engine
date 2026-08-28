layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 FragNormal;
layout(location = 2) out vec4 FragMaterial;

in vec2 textCord;
in vec3 fragPos;
in mat3 TBN;

uniform vec3 u_Color;

void main()
{
    FragColor    = vec4(u_Color, 1.0);
    FragNormal   = vec4(normalize(TBN[2]), 1.0);
    FragMaterial = vec4(0.5, 0.0, 1.0, 1.0);
}