#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
    vec4 texColor = texture(uTexture, TexCoord);
    vec4 finalColor = texColor * uColor;

    if (finalColor.a < 0.01)
        discard;

    FragColor = finalColor;
}