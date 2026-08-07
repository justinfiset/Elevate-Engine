layout(location = 0) out vec4 o_Color;

uniform sampler2D sceneTexture;
uniform sampler2D aoTexture;

in vec2 v_TexCoord;

void main()
{
    vec3 color = texture(sceneTexture, v_TexCoord).rgb;
    float ao = texture(aoTexture, v_TexCoord).r;
    o_Color = vec4(color * ao, 1.0);
}