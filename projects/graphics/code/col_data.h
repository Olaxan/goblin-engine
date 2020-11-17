#pragma once

#include "vector3.h"

#include <vector>

namespace efiilj
{
	struct mesh_data
	{
		std::vector<vector3> positions;
		std::vector<vector3> normals;
		std::vector<unsigned> indices;
	};
}
