#include "cube.h"

#include <GL/glew.h>

namespace efiilj
{
	cube::cube(const vector3& min, const vector3& max) 
		: mesh_resource() 
	{

		vector3 positions[24] = 
		{
			vector3(min.x, min.y, max.z),
			vector3(max.x, min.y, max.z),
			vector3(max.x, max.y, max.z),
			vector3(min.x, max.y, max.z),

			vector3(max.x, min.y, min.z),
			vector3(min.x, min.y, min.z),
			vector3(min.x, max.y, min.z),
			vector3(max.x, max.y, min.z),

			vector3(min.x, max.y, max.z),
			vector3(max.x, max.y, max.z),
			vector3(max.x, max.y, min.z),
			vector3(min.x, max.y, min.z),

			vector3(max.x, min.y, max.z),
			vector3(min.x, min.y, max.z),
			vector3(min.x, min.y, min.z),
			vector3(max.x, min.y, min.z),

			vector3(min.x, min.y, min.z),
			vector3(min.x, min.y, max.z),
			vector3(min.x, max.y, max.z),
			vector3(min.x, max.y, min.z),

			vector3(max.x, min.y, max.z),
			vector3(max.x, min.y, min.z),
			vector3(max.x, max.y, min.z),
			vector3(max.x, max.y, max.z)
		};

		vector3 normals[24] = 
		{
			vector3(0, 0, 1),
			vector3(0, 0, 1),
			vector3(0, 0, 1),
			vector3(0, 0, 1),

			vector3(0, 0, -1),
			vector3(0, 0, -1),
			vector3(0, 0, -1),
			vector3(0, 0, -1),
			
			vector3(0, 1, 0),
			vector3(0, 1, 0),
			vector3(0, 1, 0),
			vector3(0, 1, 0),

			vector3(0, -1, 0),
			vector3(0, -1, 0),
			vector3(0, -1, 0),
			vector3(0, -1, 0),

			vector3(-1, 0, 0),
			vector3(-1, 0, 0),
			vector3(-1, 0, 0),
			vector3(-1, 0, 0),

			vector3(1, 0, 0),
			vector3(1, 0, 0),
			vector3(1, 0, 0),
			vector3(1, 0, 0)
		};

		vector2 uvs[24] = 
		{
			vector2(0.25f, 0.33f),
			vector2(0.50f, 0.33f),
			vector2(0.50f, 0.66f),
			vector2(0.25f, 0.66f),

			vector2(0.75, 0.33f),
			vector2(1.0f, 0.33f),
			vector2(1.0f, 0.66f),
			vector2(0.75, 0.66f),

			vector2(0.25f, 0.66f),
			vector2(0.50f, 0.66f),
			vector2(0.50f, 1.0f),
			vector2(0.25f, 1.0f),
								 
			vector2(0.25f, 0.0f),
			vector2(0.50f, 0.0f),
			vector2(0.50f, 0.33f),
			vector2(0.25f, 0.33f),
			  				  
			vector2(0.00f, 0.33f),
			vector2(0.25f, 0.33f),
			vector2(0.25f, 0.66f),
			vector2(0.00f, 0.66f),
								  
			vector2(0.50f, 0.33f),
			vector2(0.75f, 0.33f),
			vector2(0.75f, 0.66f),
			vector2(0.50f, 0.66f)
		};

		unsigned int indices[36] = {
			0,  1,  3,  2,  3,  1,  // Front
			4,  5,  7,  6,  7,  5,  // Back
			8,  9,  11, 10, 11, 9,  // Top
			12, 13, 15, 14, 15, 13, // Bottom
			16, 17, 19, 18, 19, 17, // Left
			20, 21, 23, 22, 23, 21  // Right
		};

		clear();

		this->_positions.insert(_positions.end(), positions, positions + 24);
		this->_normals.insert(_normals.end(), normals, normals + 24);
		this->_uvs.insert(_uvs.end(), uvs, uvs + 24);
		this->_indices.insert(_indices.end(), indices, indices + 36);

		finalize();
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
