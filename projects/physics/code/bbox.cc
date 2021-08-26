#include "bbox.h"

#include <GL/glew.h>
#include <memory>

namespace efiilj
{
//	bbox::bbox(std::shared_ptr<physics_node> node)
//		: _node(std::move(node))
//	{
//
//		unsigned int indices[24] = {
//			0, 1,
//			0, 3,
//			0, 4,
//			2, 1,
//			2, 3,
//			2, 7,
//			6, 3,
//			6, 4,
//			6, 7,
//			5, 1,
//			5, 4,
//			5, 7
//		};
//
//		clear();
//
//		this->_indices.insert(_indices.end(), indices, indices + 24);
//
//		update_positions();
//		build(GL_DYNAMIC_DRAW);
//	}
//
//	void bbox::update_positions()
//	{
//		bounds b = _node->get_bounds();
//
//		vector3 positions[8] = {
//			vector3(b.max.x, b.min.y, b.min.z),
//			vector3(b.max.x, b.max.y, b.min.z),
//			vector3(b.min.x, b.max.y, b.min.z),
//			vector3(b.min.x, b.min.y, b.min.z),
//			vector3(b.max.x, b.min.y, b.max.z),
//			vector3(b.max.x, b.max.y, b.max.z),
//			vector3(b.min.x, b.min.y, b.max.z),
//			vector3(b.min.x, b.max.y, b.max.z)
//		};
//
//		this->_positions.clear();
//		this->_positions.insert(_positions.end(), positions, positions + 8);
//
//	}
//
//	void bbox::update()
//	{
//		update_positions();
//		buffer();
//	}
//
//	void bbox::draw_elements() const 
//	{
//		glDrawElements(GL_LINES, get_index_count(), _type, nullptr);
//	}
}
