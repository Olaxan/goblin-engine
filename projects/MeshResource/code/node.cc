#include "node.h"
#include <utility>

namespace efiilj
{
	graphics_node::graphics_node()
	= default;

	graphics_node::graphics_node(
		std::shared_ptr<mesh_resource> mesh_ptr,
		std::shared_ptr<material_base> material_ptr,
		std::shared_ptr<transform_model> transform_ptr)
			: mesh_(std::move(mesh_ptr)), material_(std::move(material_ptr)), transform_(std::move(transform_ptr)) { }

	void graphics_node::bind() const
	{
		mesh_->bind();
		material_->apply();
	}

	void graphics_node::unbind() const
	{
		mesh_->unbind();
		material_->apply();
	}

	void graphics_node::draw() const
	{
		bind();
		material_->program().set_uniform("model", transform_->model());
		mesh_->draw_elements();
		unbind();
	}
}
