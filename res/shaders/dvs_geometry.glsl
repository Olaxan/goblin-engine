#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 tangent;

out VS_OUT
{
	vec3 Fragment;
	vec2 Uv;
	mat3 TBN;
} vs_out;

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform float dt;
uniform mat4 model;

void main()
{
	vec4 world_pos = model * vec4(pos, 1.0);

	vec3 T = normalize(vec3(model * tangent));
	vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
	vec3 B = cross(N, T) * tangent.w;

	vs_out.Fragment = world_pos.xyz;
	vs_out.Uv = uv;
	vs_out.TBN = mat3(T, B, N);

	gl_Position = projection * view * world_pos;
}
