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

struct spotlight_data
{
	float inner;
	float outer;
};

struct light_source
{
	light_base base;
	vec3 position;
	vec3 direction;
	attenuation falloff;
	spotlight_data cutoff;
	int type;
};

out vec4 FragColor;

layout (location = 0) uniform sampler2D g_position;
layout (location = 1) uniform sampler2D g_normal;
layout (location = 2) uniform sampler2D g_albedo;
layout (location = 3) uniform sampler2D g_orm;
layout (location = 4) uniform sampler2D g_emissive;

uniform vec3 cam_pos;
uniform mat4 light_model;
uniform int light_type;

uniform light_source source;

const float PI = 3.14159265359;

vec3 fresnel_schlick(float cos_theta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cos_theta, 5.0);
} 

float distribution_GGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float geometry_schlick_GGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = geometry_schlick_GGX(NdotV, roughness);
    float ggx1  = geometry_schlick_GGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec4 calc_base(light_base light, vec3 direction, vec3 position, vec3 normal, vec3 albedo, vec3 orm)
{
	vec3 view_dir = normalize(cam_pos - position);
	vec3 halfway_dir = normalize(direction + view_dir);

	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, orm.z);	

	// cook-torrance brdf
	float NDF = distribution_GGX(normal, halfway_dir, orm.x);        
	float G   = geometry_smith(normal, view_dir, direction, orm.x);      
	vec3 F    = fresnel_schlick(max(dot(halfway_dir, view_dir), 0.0), F0);       
	
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - orm.z;	  
	
	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, direction), 0.0);
	vec3 specular     = numerator / max(denominator, 0.001);  
		
	float NdotL = max(dot(normal, direction), 0.0);                
	vec3 result = (kD * albedo / PI + specular) * light.color * light.diffuse_intensity * NdotL;

	return vec4(result, 1.0);
}

vec4 calc_directional(vec3 position, vec3 normal, vec3 albedo, vec3 orm)
{
	return calc_base(source.base, source.direction, position, normal, albedo, orm);
}

vec4 calc_pointlight(vec3 position, vec3 normal, vec3 albedo, vec3 orm)
{
	vec3 light_dir = source.position - position;
	float light_dist = length(light_dir);
	light_dir = normalize(light_dir);

	vec4 color = calc_base(source.base, light_dir, position, normal, albedo, orm);

	float atten = source.falloff.constant +
			source.falloff.linear * light_dist + 
			source.falloff.exponential * light_dist * light_dist;

	atten = max(1.0, atten);
	
	return color / atten;
}

vec4 calc_spotlight(vec3 position, vec3 normal, vec3 albedo, vec3 orm)
{
	vec3 light_dir = normalize(source.position - position);

	vec4 color = calc_pointlight(position, normal, albedo, orm);

	float theta = dot(light_dir, normalize(-source.direction));
	float epsilon = source.cutoff.inner - source.cutoff.outer;
	float intensity = clamp((theta - source.cutoff.outer) / max(epsilon, 0.001), 0.0, 1.0);

	return color * intensity;
}

void main()
{
	ivec2 Uv = ivec2(gl_FragCoord.xy);

	vec3 Normal = texelFetch(g_normal, Uv, 0).rgb;
	vec3 WorldPos = texelFetch(g_position, Uv, 0).rgb;
	vec3 Color = texelFetch(g_albedo, Uv, 0).rgb;
	vec3 ORM = texelFetch(g_orm, Uv, 0).rgb;
	vec4 Emissive = texelFetch(g_emissive, Uv, 0);

	if (source.type == LIGHT_DIRECTIONAL)
	{
		FragColor = calc_directional(WorldPos, Normal, Color, ORM);
	}
	else if (source.type == LIGHT_POINT)
	{
		FragColor = calc_pointlight(WorldPos, Normal, Color, ORM);
	}
	else if (source.type == LIGHT_SPOT)
	{
		FragColor = calc_spotlight(WorldPos, Normal, Color, ORM);
	}
}
