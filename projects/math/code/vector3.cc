#include "vector3.h"

namespace efiilj
{
	vector3 operator * (float lhs, const vector3& rhs)
	{
		return rhs * lhs;
	}
}
