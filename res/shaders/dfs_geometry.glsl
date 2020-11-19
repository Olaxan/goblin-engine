#version 430

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gORM;
layout (location = 4) out vec3 gEmissive;

in VS_OUT 
{
	vec3 Fragment;
	vec2 Uv;
	mat3 TBN;
} fs_in;

uniform sampler2D tex_base;
uniform sampler2D tex_normal;
uniform sampler2D tex_orm;
uniform sampler2D tex_emissive;

uniform vec4 base_color_factor;
uniform vec3 emissive_factor;
uniform float metallic_factor;
uniform float roughness_factor;
uniform float alpha_cutoff;

void main()
{
	gPosition = fs_in.Fragment;

	vec3 normal = texture(tex_normal, fs_in.Uv).rgb;
	normal = normal * 2.0 - 1.0;
	gNormal = normalize(fs_in.TBN * normal);
	
	vec4 albedo = base_color_factor * texture(tex_base, fs_in.Uv);
	vec3 orm = texture(tex_orm, fs_in.Uv).rgb;

	if (albedo.a < alpha_cutoff)
		discard;

	gAlbedo = albedo.rgb; 
	gORM = vec3(orm.x, roughness_factor * orm.y, metallic_factor * orm.z);
	gEmissive = emissive_factor * texture(tex_emissive, fs_in.Uv).xyz;
}
