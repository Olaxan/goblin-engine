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
		vector3 rgb;
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
		vector3 position;
		vector3 direction;
		attenuation falloff;
		light_type type;	
	};
}
