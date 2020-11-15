#pragma once

#include "matrix4.h"
#include "vector3.h"
#include "plane.h"

#define EPSILON 0.00001

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

			bool intersect(const plane& test, vector3& result)
			{
				float denom = vector3::dot(direction, test.normal);

				if (fabs(denom) > EPSILON)
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

			bool intersect(const plane& test, const matrix4& transform, vector3& result)
			{
				vector4 plane_offset = transform * vector4(test.offset, 1.0f);
				vector4 plane_normal = transform * vector4(test.normal, 1.0f);

				plane p(plane_offset.xyz(), plane_normal.xyz());

				return intersect(p, result);
					
			}
	};
}
