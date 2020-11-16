#pragma once

#include "plane.h"
#include "mesh_res.h"
#include "transform.h"

namespace efiilj
{
	class cube : public mesh_resource
	{
		private:

		public:

			cube(const vector3& min, const vector3& max);
			cube(const vector3& scale);
			cube(float size = 1.0f);
	};
}
