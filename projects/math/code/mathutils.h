#pragma once

#include "stdlib.h"

namespace efiilj
{
	float randf(float max = 1.0f)
	{
		return static_cast<float>(rand()) / static_cast <float> (static_cast<float>(RAND_MAX) / max);
	}

	float randf(float min, float max)
	{
		return min + static_cast<float>(rand()) / ( static_cast<float>(static_cast<float>(RAND_MAX) / (max - min)));
	}
}
