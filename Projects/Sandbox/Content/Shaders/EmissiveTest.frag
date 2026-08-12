layout(location = 0) out vec4 FragColor;

uniform vec3 u_EmissiveColor = vec3(5.0, 1.0, 0.1);

void main()
{
    FragColor = vec4(u_EmissiveColor, 1.0);
}