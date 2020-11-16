#pragma once

#include "vector3.h"

#include <vector>

namespace efiilj
{
	class mesh_data
	{
		private:

			std::vector<vector3> positions_;
			std::vector<unsigned> indices_;

		public:

			mesh_data();

	};
}
