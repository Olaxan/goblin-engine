#include "cube.h"

#include <GL/glew.h>

namespace efiilj
{
	cube::cube(const vector3& min, const vector3& max) 
		: mesh_resource() 
	{

		vertex vertices[24] = {

			// Front = S
			vertex(vector3(min.x, min.y, max.z), vector3(0, 0, 1), vector2(0.25f, 0.33f)),	// 0
			vertex(vector3(max.x, min.y, max.z), vector3(0, 0, 1), vector2(0.50f, 0.33f)),	// 1
			vertex(vector3(max.x, max.y, max.z), vector3(0, 0, 1), vector2(0.50f, 0.66f)),	// 2
			vertex(vector3(min.x, max.y, max.z), vector3(0, 0, 1), vector2(0.25f, 0.66f)),  // 3

			// Back = N
			vertex(vector3(max.x, min.y, min.z), vector3(0, 0, -1), vector2(0.75, 0.33f)), // 4
			vertex(vector3(min.x, min.y, min.z), vector3(0, 0, -1), vector2(1.0f, 0.33f)), // 5
			vertex(vector3(min.x, max.y, min.z), vector3(0, 0, -1), vector2(1.0f, 0.66f)), // 6
			vertex(vector3(max.x, max.y, min.z), vector3(0, 0, -1), vector2(0.75, 0.66f)), // 7

			// Top
			vertex(vector3(min.x, max.y, max.z), vector3(0, 1, 0), vector2(0.25f, 0.66f)), // 8
			vertex(vector3(max.x, max.y, max.z), vector3(0, 1, 0), vector2(0.50f, 0.66f)), // 9
			vertex(vector3(max.x, max.y, min.z), vector3(0, 1, 0), vector2(0.50f, 1.0f)),  // 10
			vertex(vector3(min.x, max.y, min.z), vector3(0, 1, 0), vector2(0.25f, 1.0f)),  // 11

			// Bottom
			vertex(vector3(max.x, min.y, max.z), vector3(0, -1, 0), vector2(0.25f, 0.0f)),  // 12
			vertex(vector3(min.x, min.y, max.z), vector3(0, -1, 0), vector2(0.50f, 0.0f)),  // 13
			vertex(vector3(min.x, min.y, min.z), vector3(0, -1, 0), vector2(0.50f, 0.33f)), // 14
			vertex(vector3(max.x, min.y, min.z), vector3(0, -1, 0), vector2(0.25f, 0.33f)), // 15

			// Left = W
			vertex(vector3(min.x, min.y, min.z), vector3(-1, 0, 0), vector2(0.00f, 0.33f)), // 16
			vertex(vector3(min.x, min.y, max.z), vector3(-1, 0, 0), vector2(0.25f, 0.33f)), // 17
			vertex(vector3(min.x, max.y, max.z), vector3(-1, 0, 0), vector2(0.25f, 0.66f)), // 18
			vertex(vector3(min.x, max.y, min.z), vector3(-1, 0, 0), vector2(0.00f, 0.66f)), // 19

			// Right = E
			vertex(vector3(max.x, min.y, max.z), vector3(1, 0, 0), vector2(0.50f, 0.33f)), // 20
			vertex(vector3(max.x, min.y, min.z), vector3(1, 0, 0), vector2(0.75f, 0.33f)), // 21
			vertex(vector3(max.x, max.y, min.z), vector3(1, 0, 0), vector2(0.75f, 0.66f)), // 22
			vertex(vector3(max.x, max.y, max.z), vector3(1, 0, 0), vector2(0.50f, 0.66f)), // 23
		};

		unsigned int indices[36] = {
			0,  1,  3,  2,  3,  1,  // Front
			4,  5,  7,  6,  7,  5,  // Back
			8,  9,  11, 10, 11, 9,  // Top
			12, 13, 15, 14, 15, 13, // Bottom
			16, 17, 19, 18, 19, 17, // Left
			20, 21, 23, 22, 23, 21  // Right
		};

		this->vertex_count_ = 24;
		this->index_count_ = 36;

		init_array_object();
		init_vertex_buffer(vertices, vertex_count_);
		init_index_buffer(indices, index_count_);

		bounds_ = bounds(vector3(min.x, min.y, min.z), vector3(max.x, max.y, max.z));
	} 	

	cube::cube(const vector3& scale)
		: cube(
				vector3(-scale.x / 2.0f, -scale.y / 2.0f, -scale.z / 2.0f), 
				vector3( scale.x / 2.0f,  scale.y / 2.0f,  scale.z / 2.0f)
		  )
	{ }

	cube::cube(float size)
		: cube(vector3(size, size, size))
	{ }
}
