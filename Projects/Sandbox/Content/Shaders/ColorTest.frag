layout(location = 0) out vec4 FragColor;

uniform vec3 u_Color = vec3(5.0, 1.0, 0.1);

void main()
{
    FragColor = vec4(u_Color, 1.0);
}