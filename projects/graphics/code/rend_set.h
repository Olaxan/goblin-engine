#pragma once

#include "vector3.h"

#include <cstring>
#include <string>

namespace efiilj
{
	struct renderer_settings
	{
		unsigned width = 1024;
		unsigned height = 1024;

		// Uniform names
		std::string ubo_camera = "Matrices";
		std::string u_camera = "cam_pos";
		std::string u_dt_seconds = "dt";

		// Volumes
		std::string pointlight_volume_path = "../res/volumes/v_pointlight.obj";
		std::string spotlight_volume_path = "../res/volumes/v_spotlight.obj";
		std::string default_primary_path = "../res/shaders/default_primary.sdr";
		std::string default_secondary_path = "../res/shaders/default_secondary.sdr";
	};
}
