layout(location = 0) out vec4 o_Occlusion;

uniform sampler2D gDepth;
uniform sampler2D gNormal;

uniform sampler2D noiseTexture;
uniform vec2 noiseScale;

uniform mat4 inverseProjection;
uniform mat4 projection;
uniform mat4 view;

in vec2 v_TexCoord;

const float RADIUS = 0.5;
const float BIAS = 0.025;
const int KERNEL_SIZE = 64;
uniform vec3 samples[KERNEL_SIZE];

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
    float depth = texture(gDepth, v_TexCoord).r;
    
    if (depth >= 1.0) {
        o_Occlusion = vec4(1.0);
        return;
    }

    vec3 fragPos = ReconstructViewPosition(v_TexCoord, depth);

    vec3 worldNormal = texture(gNormal, v_TexCoord).xyz * 2.0 - 1.0;
    vec3 normal = normalize(mat3(view) * worldNormal); 

    vec3 randomVec = texture(noiseTexture, v_TexCoord * noiseScale).xyz;
    randomVec = normalize(randomVec * 2.0 - 1.0);

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    
    float occlusion = 0.0;
    for (int i = 0; i < KERNEL_SIZE; i++)
    {
        vec3 sampleVec = TBN * samples[i];
        vec3 samplePos = fragPos + sampleVec * RADIUS;

        vec4 offset = projection * vec4(samplePos, 1.0);
        offset.xyz /= offset.w;
        vec2 sampleUV = offset.xy * 0.5 + 0.5;

        if (sampleUV.x < 0.0 || sampleUV.x > 1.0 || sampleUV.y < 0.0 || sampleUV.y > 1.0)
        {
            continue;
        }

        float sampleDepth = texture(gDepth, sampleUV).r;
        vec3 sampledPos = ReconstructViewPosition(sampleUV, sampleDepth);

        float dist = abs(fragPos.z - sampledPos.z);
        float rangeCheck = smoothstep(0.0, 1.0, RADIUS / dist);

        if (sampledPos.z >= samplePos.z + BIAS)
        {
            occlusion += rangeCheck;
        }
    }

    occlusion = 1.0 - (occlusion / float(KERNEL_SIZE));
    o_Occlusion = vec4(vec3(occlusion), 1.0);
}