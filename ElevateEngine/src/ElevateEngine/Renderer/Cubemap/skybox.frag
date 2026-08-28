R""(precision highp float;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FragNormal;
layout (location = 2) out vec4 FragPosition;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    vec3 col = texture(skybox, TexCoords).rgb;
    
    FragColor = vec4(col, 1.0);
    FragNormal = vec4(0.0, 0.0, 0.0, 0.0);
    FragPosition = vec4(0.0, 0.0, 0.0, 0.0);
})""