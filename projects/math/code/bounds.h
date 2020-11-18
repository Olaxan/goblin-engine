#pragma once

#include "matrix4.h"

namespace efiilj
{
	class bounds
	{
		private:

		public:

			bounds()
				: min(vector3()), max(vector3())
			{}

			bounds(const vector3& min, const vector3 max)
				: min(vector3::min(min, max)), max(vector3::max(min, max))
			{}

			vector3 min, max;

			bounds get_transformed_bounds(const matrix4& mat)
			{
				const vector3 x1(mat.col(0).xyz() * min.x);
				const vector3 x2(mat.col(0).xyz() * max.x);
				
				const vector3 y1(mat.col(1).xyz() * min.y);
				const vector3 y2(mat.col(1).xyz() * max.y);

				const vector3 z1(mat.col(2).xyz() * min.z);
				const vector3 z2(mat.col(2).xyz() * max.z);

				const vector3 t = mat.col(3).xyz();

				return bounds(
					vector3::min(x1, x2) + vector3::min(y1, y2) + vector3::min(z1, z2) + t,
					vector3::max(x1, x2) + vector3::max(y1, y2) + vector3::max(z1, z2) + t
				);
			}
	};
}
