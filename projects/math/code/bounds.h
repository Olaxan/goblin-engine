#pragma once

#include "matrix4.h"
#include "ray.h"

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

			bounds(const vector3& min, const vector3& max, const matrix4& model)
				: bounds(min, max)
			{
				bounds b = get_transformed_bounds(model);
				this->min = b.min;
				this->max = b.max;
			}

			vector3 min, max;

			bounds get_transformed_bounds(const matrix4& mat) const
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

			bool point_inside(const vector3& point, float epsilon = 0.0001f) const
			{
				return point.x > (min.x - epsilon)
					&& point.x < (max.x + epsilon)
					&& point.y > (min.y - epsilon)
					&& point.y < (max.y + epsilon)
					&& point.z > (min.z - epsilon)
					&& point.z < (max.z + epsilon);
			}

			bool ray_intersection(const ray& test, vector3& hit) const
			{
				float tmax = 100000000.0f;
				float tmin = -tmax;

				const vector3 dir = vector3(
					1.0f / ((std::fabs(test.direction.x) < 1e-6) ? 0.0001f : test.direction.x),
					1.0f / ((std::fabs(test.direction.y) < 1e-6) ? 0.0001f : test.direction.y),
					1.0f / ((std::fabs(test.direction.z) < 1e-6) ? 0.0001f : test.direction.z)
				);

				const float tx1 = (this->min.x - test.origin.x) * dir.x;
				const float tx2 = (this->max.x - test.origin.x) * dir.x;

				tmin = std::max(tmin, std::min(tx1, tx2));
				tmax = std::min(tmax, std::max(tx1, tx2));
				
				const float ty1 = (this->min.y - test.origin.y) * dir.y;
				const float ty2 = (this->max.y - test.origin.y) * dir.y;

				tmin = std::max(tmin, std::min(ty1, ty2));
				tmax = std::min(tmax, std::max(ty1, ty2));
			   
				const float tz1 = (this->min.z - test.origin.z) * dir.z;
				const float tz2 = (this->max.z - test.origin.z) * dir.z;

				tmin = std::max(tmin, std::min(tz1, tz2));
				tmax = std::min(tmax, std::max(tz1, tz2));
				
				if (tmin > 0.0f && tmax >= tmin) 
				{
					hit = test.origin + test.direction * tmin;
					return true;
				}

				return false;
			}
	};
}
