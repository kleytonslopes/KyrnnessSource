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
//void main()
//{
////    float alpha = texture(uTexture, TexCoord).r;
////    FragColor = uColor * vec4(1.0, 1.0, 1.0, alpha);
//    vec4 texColor = texture(uTexture, TexCoord);
//    FragColor = texColor * uColor; // Isso permite controle de cor e opacidade
//
//}