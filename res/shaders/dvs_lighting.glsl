#version 330 core

layout (location = 0) in vec4 pos;

uniform mat4 light_mvp;

void main()
{
    gl_Position = light_mvp * pos;
}
