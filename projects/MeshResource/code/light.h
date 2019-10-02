#pragma once

#include "transform.h"

namespace efiilj
{
	struct point_light
	{
		point_light(const int color, const float intensity, const transform_model& transform)
		: color(color), intensity(intensity), transform(transform) { }
		
		int color;
		float intensity;
		transform_model transform;
	};
}
