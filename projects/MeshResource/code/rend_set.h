#pragma once

#include "vector3.h"

#include <string>

namespace efiilj
{
	struct light_data
	{
		vector3 position = vector3(100.0f, 100.0f, 100.0f);
		vector3 color = vector3(1.0f, 1.0f, 1.0f);
		float intensity = 1.0f;
	};	

	struct renderer_settings
	{
		unsigned width = 1024;
		unsigned height = 1024;

		// Uniform names
		std::string ubo_camera = "Matrices";
		std::string u_camera = "cam_pos";
		std::string u_dt_seconds = "dt";
		std::string u_sun_color = "sun.color";
		std::string u_sun_pos = "sun.position";
		std::string u_sun_int = "sun.intensity";

		// Light
		light_data sun;
		vector3 ambient_color = vector3(1.0f, 1.0f, 1.0f);
		float ambient_strength = 0.01f;
		float specular_strength = 1.0f;	
	};
}
