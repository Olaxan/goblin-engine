#include "rect.h"

#include <GL/glew.h>

namespace efiilj
{
	rect::rect() 
		: mesh_resource() 
	{

		vector3 positions[4] = 
		{
			vector3(-1.0f, -1.0f, 0.0f),
			vector3( 1.0f, -1.0f, 0.0f),
			vector3(-1.0f,  1.0f, 0.0f),
			vector3( 1.0f,  1.0f, 0.0f)
		};

		vector3 normals[4] = 
		{
			vector3(0, 0, 1),
			vector3(0, 0, 1),
			vector3(0, 0, 1),
			vector3(0, 0, 1)
		};

		vector2 uvs[4] = 
		{
			vector2(0, 0),
			vector2(1, 0),
			vector2(1, 1),
			vector2(0, 1)
		};

		unsigned indices[] = 
		{
			0, 1, 2, 2, 1, 3	
		};

		clear();

		this->_positions.insert(_positions.end(), positions, positions + 24);
		this->_normals.insert(_normals.end(), normals, normals + 24);
		this->_uvs.insert(_uvs.end(), uvs, uvs + 24);
		this->_indices.insert(_indices.end(), indices, indices + 24);

		finalize();

		//bounds_ = bounds(vector3(-1.0f, -1.0f, 0.0f), vector3(1.0f, 1.0f, 0.0f));
	} 	
}
