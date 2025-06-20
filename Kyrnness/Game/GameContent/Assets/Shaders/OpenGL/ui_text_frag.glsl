#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
   // float alpha = texture(uTexture, TexCoord).r;
    FragColor = vec4(TexCoord.xy, 0.0, 1.0);
}