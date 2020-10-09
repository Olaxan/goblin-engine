#version 430

struct point_light
{
	vec3 color;
	vec3 intensity;
	vec3 position;
};

layout(location = 0) in vec3 fragment;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 uv;

uniform vec4 u_camera_position;
uniform sampler2D u_sampler;
uniform point_light u_light;
uniform vec3 u_ambient_color;
uniform float u_ambient_strength;
uniform float u_specular_strength;
uniform int u_shininess;

out vec4 Color;

void main()
{
    vec3 ambient = u_ambient_strength * u_ambient_color;
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(u_light.position - fragment);
	vec3 view_dir = normalize(u_camera_position.xyz - fragment);
	vec3 reflect_dir = reflect(-light_dir, norm);
	
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * u_light.color;
	
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_shininess);
	vec3 specular = u_specular_strength * spec * u_light.color;
	
	vec3 result = (ambient + diffuse + specular);
	
	Color = vec4(result, 1.0) * color * texture(u_sampler, uv);
}