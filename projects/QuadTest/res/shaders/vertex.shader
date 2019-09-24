#version 430

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec2 Uv;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * pos;
	Uv = uv;
}