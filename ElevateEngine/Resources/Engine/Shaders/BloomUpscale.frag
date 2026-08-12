uniform sampler2D u_ScreenTex;
uniform float u_FilterRadius;

in vec2 v_TexCoord;
layout (location = 0) out vec3 upsample;

void main()
{
    float x = u_FilterRadius;
    float y = u_FilterRadius;

    vec3 a = texture(u_ScreenTex, vec2(v_TexCoord.x - x, v_TexCoord.y + y)).rgb;
    vec3 b = texture(u_ScreenTex, vec2(v_TexCoord.x,     v_TexCoord.y + y)).rgb;
    vec3 c = texture(u_ScreenTex, vec2(v_TexCoord.x + x, v_TexCoord.y + y)).rgb;

    vec3 d = texture(u_ScreenTex, vec2(v_TexCoord.x - x, v_TexCoord.y)).rgb;
    vec3 e = texture(u_ScreenTex, vec2(v_TexCoord.x,     v_TexCoord.y)).rgb;
    vec3 f = texture(u_ScreenTex, vec2(v_TexCoord.x + x, v_TexCoord.y)).rgb;

    vec3 g = texture(u_ScreenTex, vec2(v_TexCoord.x - x, v_TexCoord.y - y)).rgb;
    vec3 h = texture(u_ScreenTex, vec2(v_TexCoord.x,     v_TexCoord.y - y)).rgb;
    vec3 i = texture(u_ScreenTex, vec2(v_TexCoord.x + x, v_TexCoord.y - y)).rgb;

    upsample = e*4.0;
    upsample += (b+d+f+h)*2.0;
    upsample += (a+c+g+i);
    upsample *= 1.0 / 16.0;
}