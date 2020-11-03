#version 430

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gORM;

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
	gPosition = fs_in.Fragment;

	vec3 normal = texture(tex_normal, fs_in.Uv).rgb;
	normal = normal * 2.0 - 1.0;
	gNormal = normalize(fs_in.TBN * normal);
	
	vec4 albedo = texture(tex_base, fs_in.Uv);

	if (albedo.a == 0.0)
			discard;

	gAlbedo = albedo.rgb; 
	gORM = texture(tex_orm, fs_in.Uv).rgb;
}
