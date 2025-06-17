#version 330 core

layout(location = 0) in vec2 aPos;

uniform mat4 uProjection; // Sua matriz ortográfica (ex: glm::ortho)

void main()
{
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
}