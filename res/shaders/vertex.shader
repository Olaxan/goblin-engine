#version 430

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 Fragment;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec4 Color;
layout(location = 3) out vec2 Uv;

uniform mat4 u_camera;
uniform mat4 u_model;

void main()
{
	gl_Position = u_camera * u_model * pos;
	Uv = uv;
	Color = vec4(1, 1, 1, 1);
	Normal = mat3(transpose(inverse(u_model))) * normal.xyz;
	Fragment = (u_model * pos).xyz;
}
