#include "transform.h"

#include <imgui.h>

namespace efiilj
{
	
	transform_model::transform_model(const vector3& pos, const vector3& rot, const vector3& scale)
	: model_(true), position_(pos, 1), scale_(scale, 1), rotation_(rot, 1), rot_(matrix4()),
	model_dirty_(true), inverse_dirty_(true) 
	{ }

	const matrix4& transform_model::get_model() const 
	{
		if (model_dirty_)
		{
			matrix4 t = matrix4::get_translation(position_);
			rot_ = matrix4::get_rotation_euler(rotation_);
			matrix4 s 	= matrix4::get_scale(scale_);

			model_ = t * rot_ * s;

			model_dirty_ = false;
			inverse_dirty_ = true;
		}

		if (parent_)
			model_ =  model_ * parent_->get_model();

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

	vector3 transform_model::right() const
	{
		get_model();
		return rot_.col(0).xyz();
	}

	vector3 transform_model::left() const
	{
		return right() * -1;
	}

	vector3 transform_model::up() const
	{
		get_model();
		return rot_.col(1).xyz();
	}

	vector3 transform_model::down() const
	{
		return up() * -1;
	}

	vector3 transform_model::forward() const
	{
		get_model();
		return rot_.col(2).xyz();
	}

	vector3 transform_model::backward() const
	{
		return forward() * -1;
	}

	void transform_model::draw_transform_gui()
	{
		ImGui::Text("Transform");
		if (ImGui::DragFloat3("Position", &position_.x, 0.1f)
			|| ImGui::DragFloat3("Rotation", &rotation_.x, 0.1f)
			|| ImGui::DragFloat3("Scale", &scale_.x, 0.1f))
			model_dirty_ = true;
	}
}
