layout(location = 0) out vec4 o_Color;

uniform sampler2D sceneTexture;
uniform sampler2D aoTexture;

in vec2 v_TexCoord;

void main()
{
    vec3 color = texture(sceneTexture, v_TexCoord).rgb;
    float ao = texture(aoTexture, v_TexCoord).r;
    vec3 linearColor = color * ao;

    // gama correction
    float gamma = 2.2;
    vec3 correctedColor = pow(linearColor, vec3(1.0 / gamma));

    o_Color = vec4(correctedColor, 1.0);
}