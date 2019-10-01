#pragma once

#include "matrix4.h"

namespace efiilj
{
	/// <summary>
	/// Struct to represent a single vertex with position and color.
	/// </summary>
	struct Vertex
	{
		Vertex() : xyzw(0, 0, 0, 0), normal(0, 0, 0, 0), rgba(1, 1, 1, 1), uv(0, 0) { }
		Vertex(Vector3 xyzw, Vector3 normal, Vector4 rgba, Vector2 uv) : xyzw(xyzw, 1), normal(normal, 1), rgba(rgba), uv(uv) { }

		Vector4 xyzw;
		Vector4 normal;
		Vector4 rgba;
		Vector2 uv;

		bool operator < (const Vertex other) const {
			return memcmp((void*)this, (void*)& other, sizeof(Vertex)) > 0;
		};
	};

	struct VertexFuzzyComparator
	{
		bool operator() (const Vertex& lhs, const Vertex& rhs) const
		{
			return Vector4::is_near(lhs.xyzw,	rhs.xyzw)
				&& Vector4::is_near(lhs.normal, rhs.normal)
				&& Vector4::is_near(lhs.rgba,	rhs.rgba)
				&& Vector2::is_near(lhs.uv,		rhs.uv);
		}
	};
}