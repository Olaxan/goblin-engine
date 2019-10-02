#pragma once

#include "matrix4.h"

namespace efiilj
{
	/// <summary>
	/// Struct to represent a single vertex with position and color.
	/// </summary>
	struct vertex
	{
		vertex()
		: xyzw(0, 0, 0, 0), normal(0, 0, 0, 0), rgba(1, 1, 1, 1), uv(0, 0) { }

		vertex(const Vector3& xyzw, const Vector3& normal, const Vector4& rgba, const Vector2& uv)
		: xyzw(xyzw, 1), normal(normal, 1), rgba(rgba), uv(uv) { }

		Vector4 xyzw;
		Vector4 normal;
		Vector4 rgba;
		Vector2 uv;

		bool operator <(const vertex other) const
		{
			return memcmp((void*)this, (void*)&other, sizeof(vertex)) > 0;
		};
	};

	struct vertex_fuzzy_comparator
	{
		bool operator()(const vertex& lhs, const vertex& rhs) const
		{
			return Vector4::is_near(lhs.xyzw, rhs.xyzw)
				&& Vector4::is_near(lhs.normal, rhs.normal)
				&& Vector4::is_near(lhs.rgba, rhs.rgba)
				&& Vector2::is_near(lhs.uv, rhs.uv);
		}
	};
}
