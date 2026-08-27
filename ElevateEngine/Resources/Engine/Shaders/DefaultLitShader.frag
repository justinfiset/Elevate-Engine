layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec3 o_Normal;
layout(location = 2) out vec4 o_Material; // r : Roughness, g : Metallic, b : AO, a : unused

in vec2 textCord;
in vec3 fragPos;
in mat3 TBN;

vec3 defaultColor       = vec3(1.0, 1.0, 1.0);
float defaultRoughness  = 0.5;
float defaultMetallic   = 0.0;
float defaultAO         = 1.0;

uniform sampler2D diffuseTex;
uniform int has_diffuseTex;

uniform sampler2D normalTex;
uniform int has_normalTex;

uniform sampler2D aoTex;
uniform int has_aoTex;

uniform sampler2D roughnessTex;
uniform int has_roughnessTex;

uniform sampler2D metallicTex;
uniform int has_metallicTex;

vec3 GetTextureColor(sampler2D tex, vec2 uv, vec3 fallbackColor, int hasTexture) {
    if (hasTexture == 1) {
        return texture(tex, uv).rgb;
    }
    return fallbackColor;
}

float GetTextureScalar(sampler2D tex, vec2 uv, float fallbackValue, int hasTexture) {
    if (hasTexture == 1) {
        return texture(tex, uv).r;
    }
    return fallbackValue;
}

vec3 GetNormal()
{
    if (has_normalTex == 1)
    {
        vec3 localNormal = texture(normalTex, textCord).rgb;
        localNormal = localNormal * 2.0 - 1.0; // Convert [0, 1] -> [-1, 1]
        return normalize(TBN * localNormal);
    }
    else
    {
        return normalize(TBN[2]);
    }
}

void main()
{
    // o_Color
    vec3 albedo     = GetTextureColor(diffuseTex, textCord, defaultColor, has_diffuseTex);
    // o_Normal
    vec3 unitNormal = GetNormal();
    // o_Material
    float roughness = GetTextureScalar(roughnessTex, textCord, defaultRoughness, has_roughnessTex);
    float metallic  = GetTextureScalar(metallicTex, textCord, defaultMetallic, has_metallicTex);
    float ao        = GetTextureScalar(aoTex, textCord, defaultAO, has_aoTex);

    o_Color = vec4(albedo, 1.0);
    o_Normal = unitNormal;
    o_Material = vec4(roughness, metallic, ao, 1.0);
}