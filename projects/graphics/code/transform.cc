#include "transform.h"

namespace efiilj
{

	transform_model::transform_model(const vector3& pos, const vector3& rot, const vector3& scale)
	: model_(true), position_(pos, 1), scale_(scale, 1), rotation_(rot, 1),
	rotation_m_(matrix4()), position_m_(matrix4()), scale_m_(matrix4()),
	model_dirty_(true), inverse_dirty_(true) 
	{ }

	const matrix4& transform_model::get_model() const 
	{
		if (model_dirty_)
		{
			position_m_ = matrix4::get_translation(position_);
			rotation_m_ = matrix4::get_rotation_euler(rotation_);
			scale_m_ 	= matrix4::get_scale(scale_);

			if (parent_ != nullptr)
			{
				parent_->get_model();
				position_m_ = parent_->position_m_ * position_m_;
				rotation_m_ = parent_->rotation_m_ * rotation_m_;
				scale_m_ 	= parent_->scale_m_ * scale_m_;
			}

			model_ = position_m_ * rotation_m_ * scale_m_;

			model_dirty_ = false;
			inverse_dirty_ = true;
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

	vector3 transform_model::right() const
	{
		get_model();
		return rotation_m_.col(0).xyz();
	}

	vector3 transform_model::left() const
	{
		return right() * -1;
	}

	vector3 transform_model::up() const
	{
		get_model();
		return rotation_m_.col(1).xyz();
	}

	vector3 transform_model::down() const
	{
		return up() * -1;
	}

	vector3 transform_model::forward() const
	{
		get_model();
		return rotation_m_.col(2).xyz();
	}

	vector3 transform_model::backward() const
	{
		return forward() * -1;
	}
}
