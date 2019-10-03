#pragma once

#include "matrix4.h"

namespace efiilj
{
	struct point_light
	{
		point_light(const vector3& rgb, const vector3& intensity, const vector3& position)
		: rgb(rgb), position(position), intensity(intensity) { }
		
		vector3 rgb;
		vector3 position;
		vector3 intensity;
	};
}
