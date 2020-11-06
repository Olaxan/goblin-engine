#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 tangent;

out VS_OUT
{
	vec3 Fragment;
	vec2 Uv;
} vs_out;

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform float time;
uniform float deltatime;
uniform mat4 model;

void main()
{
	vec4 mod_pos = model * vec4(pos, 1.0);
	gl_Position = projection * view * mod_pos;

	vs_out.Fragment = mod_pos.xyz;
	vs_out.Uv = uv;
}
