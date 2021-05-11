#pragma once

#include "matrix4.h"
#include "string.h"

namespace efiilj
{
	
	/**
	 * \brief Struct to represent a single vertex with position, normals, and UV.
	 */
	struct vertex
	{
		vertex()
		: xyzw(0, 0, 0), normal(0, 0, 0), uv(0, 0) { }

		vertex(const vector3& xyzw, const vector3& normal, const vector2& uv)
		: xyzw(xyzw), normal(normal), uv(uv) { }

		vector3 xyzw;
		vector3 normal;
		vector2 uv;

		bool operator < (const vertex& other) const
		{
			return memcmp(this, &other, sizeof(vertex)) > 0;
		};
	};

	//struct vertex_fuzzy_comparator
	//{
	//	bool operator()(const vertex& lhs, const vertex& rhs) const
	//	{
	//		return vector3::is_near(lhs.xyzw, rhs.xyzw)
	//			&& vector3::is_near(lhs.normal, rhs.normal)
	//			&& vector2::is_near(lhs.uv, rhs.uv);
	//	}
	//};
}
