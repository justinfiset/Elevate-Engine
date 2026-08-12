uniform sampler2D u_ScreenTex;
uniform vec2 u_ScreenRes;

in vec2 v_TexCoord;
layout (location = 0) out vec3 downsample;

void main()
{
    vec2 srcTexelSize = 1.0 / u_ScreenRes;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    vec3 a = texture(u_ScreenTex, vec2(v_TexCoord.x - 2*x, v_TexCoord.y + 2*y)).rgb;
    vec3 b = texture(u_ScreenTex, vec2(v_TexCoord.x,       v_TexCoord.y + 2*y)).rgb;
    vec3 c = texture(u_ScreenTex, vec2(v_TexCoord.x + 2*x, v_TexCoord.y + 2*y)).rgb;

    vec3 d = texture(u_ScreenTex, vec2(v_TexCoord.x - 2*x, v_TexCoord.y)).rgb;
    vec3 e = texture(u_ScreenTex, vec2(v_TexCoord.x,       v_TexCoord.y)).rgb;
    vec3 f = texture(u_ScreenTex, vec2(v_TexCoord.x + 2*x, v_TexCoord.y)).rgb;

    vec3 g = texture(u_ScreenTex, vec2(v_TexCoord.x - 2*x, v_TexCoord.y - 2*y)).rgb;
    vec3 h = texture(u_ScreenTex, vec2(v_TexCoord.x,       v_TexCoord.y - 2*y)).rgb;
    vec3 i = texture(u_ScreenTex, vec2(v_TexCoord.x + 2*x, v_TexCoord.y - 2*y)).rgb;

    vec3 j = texture(u_ScreenTex, vec2(v_TexCoord.x - x, v_TexCoord.y + y)).rgb;
    vec3 k = texture(u_ScreenTex, vec2(v_TexCoord.x + x, v_TexCoord.y + y)).rgb;
    vec3 l = texture(u_ScreenTex, vec2(v_TexCoord.x - x, v_TexCoord.y - y)).rgb;
    vec3 m = texture(u_ScreenTex, vec2(v_TexCoord.x + x, v_TexCoord.y - y)).rgb;

    downsample = e*0.125;
    downsample += (a+c+g+i)*0.03125;
    downsample += (b+d+f+h)*0.0625;
    downsample += (j+k+l+m)*0.125;
}