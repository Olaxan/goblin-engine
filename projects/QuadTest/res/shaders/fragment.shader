#version 430

layout(location = 0) in vec2 uv;

uniform sampler2D u_sampler;

out vec4 Color;

void main()
{
	Color = texture(u_sampler, uv);
}