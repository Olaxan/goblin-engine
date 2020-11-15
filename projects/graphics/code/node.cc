#include "node.h"
#include <memory>
#include <utility>

namespace efiilj
{

	graphics_node::graphics_node(
		std::shared_ptr<mesh_resource> mesh_ptr,
		std::shared_ptr<material_base> material_ptr,
		std::shared_ptr<transform_model> transform_ptr)
			: mesh_(std::move(mesh_ptr)), material_(std::move(material_ptr)), transform_(std::move(transform_ptr)), is_absolute_(false) 
	{}

	graphics_node::graphics_node(
			std::shared_ptr<mesh_resource> mesh_ptr,
			std::shared_ptr<material_base> material_ptr)
		: graphics_node(mesh_ptr, material_ptr, std::make_shared<transform_model>())
	{}

	void graphics_node::bind() const
	{
		mesh_->bind();

		if (material_ != nullptr)
			material_->apply();
	}

	void graphics_node::unbind() const
	{
		mesh_->unbind();
	}

	void graphics_node::draw() const
	{
		matrix4 model = is_absolute_ ? matrix4() : transform_->get_model();
		material_->get_program()->set_uniform("model", model);
		mesh_->draw_elements();
	}

	bool graphics_node::point_inside_bounds(const vector3& point)
	{
		bounds b = mesh_->get_bounds(transform_->get_model());

		return point.x > b.min.x && point.x < b.max.x && point.y > b.min.y && point.y < b.max.y && point.z > b.min.z && point.z < b.max.z;
	}
}
