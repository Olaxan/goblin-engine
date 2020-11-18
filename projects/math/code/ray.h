#pragma once

#include "matrix4.h"
#include "vector3.h"
#include "plane.h"

namespace efiilj
{
	class ray
	{
		private:

		public:

			ray(const vector3& origin, const vector3& direction)
			{
				this->origin = origin;
				this->direction = direction;
			}

			vector3 origin, direction;

			bool intersect(const plane& test, vector3& result) const
			{
				float denom = vector3::dot(direction, test.normal);

				if (fabs(denom) > 1e-6)
				{
					vector3 pl = test.offset - origin;
					float t = vector3::dot(pl, test.normal) / denom;

					if (t < 0)
						return false;

					result = origin + direction * t;

					return true;
				}

				return false;
			}

			bool intersect(const plane& test, const matrix4& transform, vector3& result) const
			{
				vector3 plane_offset = transform * test.offset;
				vector3 plane_normal = transform * test.normal;

				plane p(plane_offset, plane_normal);

				return intersect(p, result);
					
			}
	};
}
