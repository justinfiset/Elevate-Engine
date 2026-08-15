uniform sampler2D u_ScreenTex;
uniform vec2 u_ScreenRes;

uniform int u_MipLevel = 0;
uniform float u_Threshold = 1.0;
uniform float u_SoftThreshold = 0.5;

in vec2 v_TexCoord;
layout (location = 0) out vec3 downsample;

vec3 QuadraticThreshold(vec3 color, float threshold, vec3 curve)
{
    float brightness = max(color.r, max(color.g, color.b));
    float rq = clamp(brightness - curve.x, 0.0, curve.y);
    float knee = curve.z * rq * rq;
    float response = max(brightness - threshold, knee) / max(brightness, 0.0001);
    return color * response;
}

void main()
{
    vec2 srcTexelSize = 1.0 / u_ScreenRes;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    // 13-tap sampling
    vec3 a = texture(u_ScreenTex, vec2(v_TexCoord.x - 2*x, v_TexCoord.y + 2*y)).rgb;
    vec3 b = texture(u_ScreenTex, vec2(v_TexCoord.x,       v_TexCoord.y + 2*y)).rgb;
    vec3 c = texture(u_ScreenTex, vec2(v_TexCoord.x + 2*x, v_TexCoord.y + 2*y)).rgb;

    vec3 d = texture(u_ScreenTex, vec2(v_TexCoord.x - 2*x, v_TexCoord.y)).rgb;
    vec3 e = texture(u_ScreenTex, vec2(v_TexCoord.x,       v_TexCoord.y)).rgb;
    vec3 f = texture(u_ScreenTex, vec2(v_TexCoord.x + 2*x, v_TexCoord.y)).rgb;

    vec3 g = texture(u_ScreenTex, vec2(v_TexCoord.x - 2*x, v_TexCoord.y - 2*y)).rgb;
    vec3 h = texture(u_ScreenTex, vec2(v_TexCoord.x,       v_TexCoord.y - 2*y)).rgb;
    vec3 i = texture(u_ScreenTex, vec2(v_TexCoord.x + 2*x, v_TexCoord.y - 2*y)).rgb;

    vec3 j = texture(u_ScreenTex, vec2(v_TexCoord.x - x,   v_TexCoord.y + y)).rgb;
    vec3 k = texture(u_ScreenTex, vec2(v_TexCoord.x + x,   v_TexCoord.y + y)).rgb;
    vec3 l = texture(u_ScreenTex, vec2(v_TexCoord.x - x,   v_TexCoord.y - y)).rgb;
    vec3 m = texture(u_ScreenTex, vec2(v_TexCoord.x + x,   v_TexCoord.y - y)).rgb;

    vec3 result = e * 0.125;
    result += (a + c + g + i) * 0.03125;
    result += (b + d + f + h) * 0.0625;
    result += (j + k + l + m) * 0.125;

    if (u_MipLevel == 0)
    {
        float knee = u_Threshold * u_SoftThreshold;
        vec3 curve = vec3(u_Threshold - knee, knee * 2.0, 0.25 / max(knee, 0.00001));

        result = QuadraticThreshold(result, u_Threshold, curve);
    }

    downsample = result;
}