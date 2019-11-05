#include "node.h"
#include <utility>

namespace efiilj
{
	graphics_node::graphics_node()
	= default;

	graphics_node::graphics_node(
		std::shared_ptr<mesh_resource> mesh_ptr,
		std::shared_ptr<texture_resource> texture_ptr,
		std::shared_ptr<shader_resource> shader_ptr,
		std::shared_ptr<transform_model> transform_ptr,
		std::shared_ptr<camera_model> camera_ptr)
		: mesh_(std::move(mesh_ptr)), texture_(std::move(texture_ptr)), shader_(std::move(shader_ptr)), transform_(
			  std::move(transform_ptr)), camera_(std::move(camera_ptr))
	{
	}

	void graphics_node::bind() const
	{
		mesh_->bind();
		texture_->bind();
		shader_->use();
	}

	void graphics_node::unbind() const
	{
		mesh_->unbind();
		texture_->unbind();
		shader_->drop();
	}

	void graphics_node::draw() const
	{
		bind();
		shader_->set_uniform("u_camera", camera_->view_perspective());
		shader_->set_uniform("u_model", transform_->model());
		mesh_->draw_elements();
		unbind();
	}
}
