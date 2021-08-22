#include "transform.h"

#include <imgui.h>

namespace efiilj
{
	
	transform_model::transform_model(const vector3& pos, const vector3& rot, const vector3& scale)
	: model_(true), position_(pos, 1), scale_(scale, 1), rot_(matrix4()),
	model_dirty_(true), inverse_dirty_(true) 
	{ }
		
	transform_model::transform_model(const transform_model& copy)
		: transform_model(copy.get_position(), vector3(), copy.get_scale())
	{ }

	const matrix4& transform_model::get_model() const 
	{
		if (true || model_dirty_)
		{
			matrix4 t = matrix4::get_translation(position_);
			rot_ = _rotation.get_rotation_matrix();
			matrix4 s 	= matrix4::get_scale(scale_);

			model_ = t * rot_ * s;

			model_dirty_ = false;
			inverse_dirty_ = true;
		}

		if (parent_ != nullptr)
		{
			model_ = parent_->get_model() * model_;
		}

		return model_;
	}

	const matrix4& transform_model::get_model_inv() const
	{
		if (inverse_dirty_)
		{
			inverse_ = model_.inverse();
			inverse_dirty_ = false;
		}

		return inverse_;
	}

	matrix4 transform_model::get_hierarchical_rotation() const
	{
		return (parent_ == nullptr) ? rot_ : parent_->get_hierarchical_rotation() * rot_;
	}

	vector3 transform_model::left() const
	{
		get_model();
		return model_.col(0).xyz().norm();
	}

	vector3 transform_model::right() const
	{
		return left() * -1;
	}

	vector3 transform_model::up() const
	{
		get_model();
		return model_.col(1).xyz().norm();
	}

	vector3 transform_model::down() const
	{
		return up() * -1;
	}

	vector3 transform_model::forward() const
	{
		get_model();
		return model_.col(2).xyz().norm();
	}

	vector3 transform_model::backward() const
	{
		return forward() * -1;
	}

	void transform_model::draw_transform_gui()
	{
		ImGui::Text("Transform");
		if (ImGui::DragFloat3("Position", &position_.x, 0.1f)
			|| ImGui::DragFloat3("Scale", &scale_.x, 0.1f))
			model_dirty_ = true;

		ImGui::Text("Rotation matrix");
		ImGui::InputFloat4("X", &rot_(0, 0));
		ImGui::InputFloat4("Y", &rot_(1, 0));
		ImGui::InputFloat4("Z", &rot_(2, 0));
		ImGui::InputFloat4("W", &rot_(3, 0));

		ImGui::Text("Model matrix");
		ImGui::InputFloat4("X", &model_(0, 0));
		ImGui::InputFloat4("Y", &model_(1, 0));
		ImGui::InputFloat4("Z", &model_(2, 0));
		ImGui::InputFloat4("W", &model_(3, 0));
	}
}
