#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 tangent;

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

uniform float time;
uniform float deltatime;
uniform mat4 model;

void main()
{
	vec4 mod_pos = model * vec4(pos, 1.0);
	//mod_pos.z = mod_pos.z - (1 + 2 * sin(time + mod_pos.x)) * 1;
	//mod_pos.y = mod_pos.y - (1 + 2 * cos(time + mod_pos.x)) * 0.2;
	gl_Position = projection * view * mod_pos;

	vec3 T = normalize(vec3(model * tangent));
	vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
	vec3 B = cross(N, T) * tangent.w;

	vs_out.Fragment = mod_pos.xyz;
	vs_out.Uv = uv;
	vs_out.TBN = mat3(T, B, N);
}
