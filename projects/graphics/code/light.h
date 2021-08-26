#pragma once

#include "matrix4.h"

namespace efiilj
{

	enum class light_type
	{
		directional = 0,
		pointlight = 1,
		spotlight = 2
	};

	struct light_base
	{
		vector3 color;
		float ambient_intensity;
		float diffuse_intensity;
	};

	struct attenuation_data
	{
		float constant;
		float linear;
		float exponential;
	};

	struct cutoff_data
	{
		float inner_angle;
		float outer_angle;
	};
}
