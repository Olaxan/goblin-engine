#version 430

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

struct light_base
{
	vec3 color;
	float ambient_intensity;
	float diffuse_intensity;
};

struct attenuation
{
	float constant;
	float linear;
	float exponential;
};

struct light_source
{
	light_base base;
	vec4 position;
	vec4 direction;
	attenuation falloff;
	int type;
};

out vec4 FragColor;

layout (location = 0) uniform sampler2D g_position;
layout (location = 1) uniform sampler2D g_normal;
layout (location = 2) uniform sampler2D g_albedo;
layout (location = 3) uniform sampler2D g_orm;

uniform vec4 cam_pos;
uniform mat4 light_model;
uniform int light_type;

uniform light_source source;

vec4 calc_base(light_base light, vec3 direction, vec3 position, vec3 normal, vec3 orm)
{
    vec4 ambient = vec4(light.color * light.ambient_intensity, 1.0);
	float diff_factor = dot(normal, direction); 

	if (diff_factor <= 0.0)
		return ambient;

	vec4 diffuse = vec4(light.color * light.diffuse_intensity * diff_factor, 1.0);
	vec3 view_dir = normalize(cam_pos.xyz - position);
	vec3 halfway_dir = normalize(direction + view_dir);

	float specular_factor = dot(normal, halfway_dir);

	if (specular_factor <= 0.0)
		return ambient * diffuse;

	float specular_power = pow(specular_factor, 2 / pow(orm.x, 4.0) - 2);

	vec4 specular = vec4(light.color * specular_power, 1.0);

	return ambient * diffuse * specular;
}

vec4 calc_directional(vec3 position, vec3 normal, vec3 orm)
{
	return calc_base(source.base, source.direction.xyz, position, normal, orm);
}

vec4 calc_pointlight(vec3 position, vec3 normal, vec3 orm)
{
	vec3 light_dir = position - source.position.xyz;
	float light_dist = length(light_dir);
	light_dir = normalize(light_dir);

	vec4 color = calc_base(source.base, light_dir, position, normal, orm);

	float atten = source.falloff.constant +
			source.falloff.linear * light_dist + 
			source.falloff.exponential * light_dist * light_dist;

	atten = max(1.0, atten);
	
	return color / atten;
}

void main()
{
	ivec2 Uv = ivec2(gl_FragCoord.xy);

	vec3 Normal = texelFetch(g_normal, Uv, 0).rgb;
	vec3 WorldPos = texelFetch(g_position, Uv, 0).rgb;
	vec3 Color = texelFetch(g_albedo, Uv, 0).rgb;
	vec3 ORM = texelFetch(g_orm, Uv, 0).rgb;

	if (source.type == LIGHT_DIRECTIONAL)
	{
		FragColor = vec4(Color, 1.0) * calc_directional(WorldPos, Normal, ORM);
	}
	else if (source.type == LIGHT_POINT)
	{
		FragColor = vec4(Color, 1.0) * calc_pointlight(WorldPos, Normal, ORM);
	}
}
