#include "vector3_op.h"

namespace efiilj
{
	vector3 operator * (float lhs, const vector3& rhs)
	{
		return rhs * lhs;
	}
}
