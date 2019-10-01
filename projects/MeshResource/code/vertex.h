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
	};
}