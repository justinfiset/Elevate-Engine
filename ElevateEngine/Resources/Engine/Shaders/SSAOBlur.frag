layout(location = 0) out vec4 o_Occlusion;

uniform sampler2D aoTexture;
uniform sampler2D gDepth;

in vec2 v_TexCoord;

void main()
{
    vec2 texelSize = vec2(1.0) / vec2(textureSize(aoTexture, 0));
    float centerDepth = texture(gDepth, v_TexCoord).r;
    
    if (centerDepth >= 1.0)
    {
        o_Occlusion = vec4(1.0);
        return;
    }

    float occlusion = 0.0;
    float totalWeight = 0.0;

    for (int x = -2; x < 2; ++x)
    {
        for (int y = -2; y < 2; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            vec2 sampleUV = v_TexCoord + offset;

            float neighborDepth = texture(gDepth, sampleUV).r;
            float sampleAO = texture(aoTexture, sampleUV).r;

            float depthDiff = abs(centerDepth - neighborDepth);
            float weight = 1.0 / (1.0 + depthDiff * 500.0); // 500.0 est la sensibilité aux bords

            occlusion += sampleAO * weight;
            totalWeight += weight;
        }
    }

    occlusion /= totalWeight;
    o_Occlusion = vec4(vec3(occlusion), 1.0);
}