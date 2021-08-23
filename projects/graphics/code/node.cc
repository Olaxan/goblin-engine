#include "node.h"

#include <memory>
#include <utility>
#include <limits>
#include <algorithm>

#include <imgui.h>

namespace efiilj
{

	graphics_node::graphics_node(

			std::shared_ptr<transform_manager> trf_mgr,
			std::shared_ptr<mesh_resource> mesh_ptr,
			std::shared_ptr<material_base> material_ptr,
			transform_id trf)
		: _transforms(std::move(trf_mgr)),
		mesh_(std::move(mesh_ptr)), 
		material_(std::move(material_ptr)), 
		_transform(trf),
		is_absolute_(false), 
		name("Object")
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
		matrix4 model = is_absolute_ ? matrix4() : _transforms->get_model(_transform);
		material_->get_program()->set_uniform("model", model);
		mesh_->update();
		mesh_->draw_elements();
	}

	void graphics_node::draw_node_gui()
	{
		ImGui::Text("Node");
	}
}
