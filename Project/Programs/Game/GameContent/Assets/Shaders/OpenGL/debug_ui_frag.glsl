#version 330 core

uniform vec4 uColor; // Cor RGBA da linha

out vec4 FragColor;

void main()
{
    FragColor = uColor;
}