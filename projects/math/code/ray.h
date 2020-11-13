#pragma once

#pragma once

#include "vector4.h"
#include "plane.h"

namespace efiilj
{
	class ray
	{
		private:


		public:

			ray(const vector4& origin, const vector4& direction)
			{
				start = origin;
				dir = direction;
			}

			ray(const vector3& origin, const vector3& direction)
			{
				start = vector4(origin, 1.0f);
				dir = vector4(direction, 1.0f);
			}

			vector4 start, dir;

			vector4 intersect(const plane& plane)
			{
				//float denom = 	
				return vector4();
			}
	};
}
