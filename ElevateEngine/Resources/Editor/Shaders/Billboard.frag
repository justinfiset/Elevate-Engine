precision highp float;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 FragNormal;
layout(location = 2) out vec4 FragMaterial;

in vec2 textCord;
uniform sampler2D billboardTexture;
uniform vec4 colorModifier;

vec4 outlineColor = vec4(0.0, 0.0, 0.0, 0.8);

void main()
{
    vec4 texColor = texture(billboardTexture, textCord);

    if (texColor.a < 0.1)
    {
        discard;
    }

    vec4 modifiedColor = texColor * colorModifier;

    if (texColor.a < 0.95)
    {
        vec2 texelSize = vec2(1.0) / vec2(textureSize(billboardTexture, 0));
        float alphaSum = 0.0;

        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                if (x == 0 && y == 0) continue;
                alphaSum += texture(billboardTexture, textCord + vec2(x, y) * texelSize).a;
            }
        }

        float outlineAlpha = clamp(alphaSum / 8.0, 0.0, 1.0);

        if (outlineAlpha > 0.0)
        {
            vec4 finalOutline = vec4(outlineColor.rgb, outlineColor.a * outlineAlpha);
            FragColor = mix(finalOutline, modifiedColor, texColor.a);
            
            FragNormal   = vec4(0.0, 0.0, 1.0, 1.0);
            FragMaterial = vec4(1.0, 0.0, 1.0, 1.0);
            return;
        }
    }

    FragColor = modifiedColor;
    
    FragNormal   = vec4(0.0, 0.0, 1.0, 1.0);
    FragMaterial = vec4(1.0, 0.0, 1.0, 1.0);
}