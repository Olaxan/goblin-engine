#version 430

layout(position = 0) out vec3 gPosition;
layout(position = 1) out vec3 gNormal;
layout(position = 2) out vec4 gAlbedoSpec;

in VS_OUT 
{
	vec3 Fragment;
	vec2 Uv;
	mat3 TBN;
} fs_in;

uniform sampler2D tex_base;
uniform sampler2D tex_normal;
uniform sampler2D tex_orm;

void main()
{
	gPosition = Fragment;

	vec3 normal = texture(tex_normal, fs_in.Uv).rgb;
	normal = normal * 2.0 - 1.0;
	gNormal = normalize(fs_in.TBN * normal);

	gAlbedoSpec.rgb = texture(tex_base, Uv).rgb;
	gAlbedoSpec.a = texture(texture_specular1, Uv).a;
}
