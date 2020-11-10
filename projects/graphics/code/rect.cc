#include "rect.h"

#include <GL/glew.h>

namespace efiilj
{
	rect::rect() 
		: mesh_resource() 
	{
		vertex quad[] = {
					vertex(vector3(-1.0f, -1.0f, 0.0f), vector3(0, 0, 1), vector2(0, 0)),
					vertex(vector3( 1.0f, -1.0f, 0.0f), vector3(0, 0, 1), vector2(1, 0)),
					vertex(vector3(-1.0f,  1.0f, 0.0f), vector3(0, 0, 1), vector2(1, 1)),
					vertex(vector3( 1.0f,  1.0f, 0.0f), vector3(0, 0, 1), vector2(0, 1))
				};

		unsigned indices[] = 
		{
			0, 1, 2, 2, 1, 3	
		};

		this->vertex_count_ = 4;
		this->index_count_ = 6;

		init_array_object();
		init_vertex_buffer(quad, 4);
		init_index_buffer(indices, 6);
	} 	

}
