out vec4 FragColor;

in vec2 textCord;
uniform sampler2D billboardTexture;

void main()
{
    vec4 texColor = texture(billboardTexture, textCord);
    if (texColor.a < 0.1)
    {
        discard;
    }
    FragColor = texColor;
}