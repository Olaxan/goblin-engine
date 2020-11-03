#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

out vec2 Uv;

uniform mat4 light_model;

void main()
{
    Uv = uv;
    gl_Position = light_model * vec4(pos, 1.0);
}
