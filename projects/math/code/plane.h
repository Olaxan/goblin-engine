#pragma once

#include "vector4.h"

namespace efiilj
{
	class plane
	{
		private:


		public:

			plane(const vector3& offset, const vector3& normal)
			{
				this->offset = offset;
				this->normal = normal;
			}

			vector3 offset, normal;
	};
}
