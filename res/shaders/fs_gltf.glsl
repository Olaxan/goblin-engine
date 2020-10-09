#version 430

struct point_light
{
	vec3 color;
	vec3 intensity;
	vec3 position;
};

in VS_OUT 
{
	vec3 Fragment;
	vec2 Uv;
	mat3 TBN;
} fs_in;

uniform vec4 camera_position;

uniform sampler2D tex_base;
uniform sampler2D tex_normal;
uniform sampler2D tex_orm;

uniform point_light light;
uniform vec3 ambient_color;
uniform float ambient_strength;
uniform float specular_strength;

out vec4 Color;

void main()
{
    	vec3 ambient = ambient_strength * ambient_color;

	vec3 normal = texture(tex_normal, fs_in.Uv).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(fs_in.TBN * normal);

	vec3 light_dir = normalize(light.position - fs_in.Fragment);
	vec3 view_dir = normalize(camera_position.xyz - fs_in.Fragment);
	vec3 halfway_dir = normalize(light_dir + view_dir);
	
	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = diff * light.color;

	float specular_power = max(1.0, 2 / pow(texture(tex_orm, fs_in.Uv).g, 4.0) - 2);	
	float spec = pow(max(dot(normal, halfway_dir), 0.0), specular_power);
	vec3 specular = specular_strength * spec * light.color;
	
	vec3 result = (ambient + diffuse + specular);
	
	Color = vec4(result, 1.0) * texture(tex_base, fs_in.Uv) * max(1.0, texture(tex_orm, fs_in.Uv).r);
}
