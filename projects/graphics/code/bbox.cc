#include "bbox.h"

#include <GL/glew.h>

namespace efiilj
{
	bbox::bbox(const vector3& min, const vector3& max)
	{

		vector3 positions[8] = {
			vector3(max.x, min.y, min.z),
			vector3(max.x, max.y, min.z),
			vector3(min.x, max.y, min.z),
			vector3(min.x, min.y, min.z),
			vector3(max.x, min.y, max.z),
			vector3(max.x, max.y, max.z),
			vector3(min.x, min.y, max.z),
			vector3(min.x, max.y, max.z)
		};

		unsigned int indices[24] = {
			0, 1,
			0, 3,
			0, 4,
			2, 1,
			2, 3,
			2, 7,
			6, 3,
			6, 4,
			6, 7,
			5, 1,
			5, 4,
			5, 7
		};

		clear();

		this->_positions.insert(_positions.end(), positions, positions + 8);
		this->_indices.insert(_indices.end(), indices, indices + 24);

		finalize();
	}

	void bbox::draw_elements() const 
	{
		glDrawElements(GL_LINES, index_count(), _type, nullptr);
	}
}
