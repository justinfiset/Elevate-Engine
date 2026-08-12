layout(location = 0) out vec4 o_Color;

uniform sampler2D sceneTexture;
uniform sampler2D aoTexture;

uniform sampler2D bloomTexture;
uniform float bloomStrength;

in vec2 v_TexCoord;

// ACES Filmic Tone Mapping Curve
vec3 ACESFilm(vec3 x)
{
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}
    
void main()
{
    vec3 hdrColor = texture(sceneTexture, v_TexCoord).rgb;
    float ao = texture(aoTexture, v_TexCoord).r;
    vec3 bloomColor = texture(bloomTexture, v_TexCoord).rgb;

    hdrColor *= ao;
    hdrColor += bloomColor * bloomStrength;
    
    vec3 mappedColor = ACESFilm(hdrColor);

    // gama correction
    float gamma = 2.2;
    vec3 correctedColor = pow(mappedColor, vec3(1.0 / gamma));

    o_Color = vec4(correctedColor, 1.0);
}