#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 UV;
out vec3 Position;

void main()
{
    // gl_Position = vec4(position.x, position.y, position.z, 1.0);
    UV = uv;
    Position = vec3(model * vec4(position, 1.0));
    gl_Position = projection * view * model * vec4(position, 1.0);
}