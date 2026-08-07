layout(location = 0) out float o_Occlusion;

uniform sampler2D gDepth;
uniform sampler2D gNormal;

uniform mat4 inverseProjection;

in vec2 v_TexCoord;

vec3 ReconstructViewPosition(vec2 uv, float depth)
{
    vec4 clipPos = vec4(
        uv * 2.0 - 1.0,
        depth * 2.0 - 1.0,
        1.0
    );

    vec4 viewPos = inverseProjection * clipPos;

    return viewPos.xyz / viewPos.w;
}

void main()
{
    float depth = texture(gDepth, v_TexCoord).r; // Get depth value from gDepth texture
    vec3 fragPos = ReconstructViewPosition(v_TexCoord, depth); // Reconstruct view space position
    vec3 normal = normalize(texture(gNormal, v_TexCoord).xyz); // Get normal from gNormal texture

    o_Occlusion = 1.0f; // temp for tests
}