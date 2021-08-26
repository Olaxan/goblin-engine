#version 330

in VS_OUT 
{
	vec3 Fragment;
	vec2 Uv;
	mat3 TBN;
} fs_in;

uniform vec3 cam_pos;

uniform sampler2D tex_base;
uniform sampler2D tex_normal;
uniform sampler2D tex_orm;

out vec4 Color;

void main()
{
	Color = texture(tex_base, fs_in.Uv) * texture(tex_orm, fs_in.Uv).r;
}
