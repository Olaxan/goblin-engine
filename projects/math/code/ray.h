#include "vector4.h"
#include "plane.h"

namespace efiilj
{
	class ray
	{
		private:

			vector4 start_, end_;

		public:

			ray(const vector4& a, const vector4& b)
			{
				start_ = a;
				end_ = b;
			}

			vector4 intersect(const plane& plane)
			{
				//float denom = 	
				return vector4();
			}
	};
}
