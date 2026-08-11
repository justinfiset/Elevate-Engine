out vec4 FragColor;

in vec2 textCord;
uniform sampler2D billboardTexture;
uniform vec4 outlineColor = vec4(0.0, 0.0, 0.0, 0.8);

void main()
{
    vec4 texColor = texture(billboardTexture, textCord);

    // Si le pixel n'est pas complètement opaque
    if (texColor.a < 0.95)
    {
        vec2 texelSize = 1.0 / textureSize(billboardTexture, 0);
        float alphaSum = 0.0;

        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                if (x == 0 && y == 0) continue; // On ne compte pas le pixel au centre
                alphaSum += texture(billboardTexture, textCord + vec2(x, y) * texelSize).a;
            }
        }

        // On normalise alphaSum par rapport au nombre de voisins (8)
        // et on applique un facteur d'intensité pour régler l'épaisseur/l'adoucissement
        float outlineAlpha = clamp(alphaSum / 8.0, 0.0, 1.0);

        if (outlineAlpha > 0.0)
        {
            // Couleur du contour avec alpha adouci
            vec4 finalOutline = vec4(outlineColor.rgb, outlineColor.a * outlineAlpha);

            // Mélange propre (Alpha Blending) entre le contour et le pixel existant
            FragColor = mix(finalOutline, texColor, texColor.a);
            return;
        }
    }

    FragColor = texColor;
}