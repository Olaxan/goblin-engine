#include "vector4.h"

namespace efiilj
{
	class plane
	{
		private:

			vector4 point_, normal_;

		public:

			plane(const vector4& point, const vector4& normal)
			{
				point_ = point;
				normal_ = normal;
			}
	};
}
