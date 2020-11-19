#version 430

in VS_OUT 
{
	vec3 Fragment;
	vec2 Uv;
} fs_in;

uniform vec4 camera_position;
uniform vec4 base_color_factor;

out vec4 Color;

void main()
{
	Color = base_color_factor;
}
