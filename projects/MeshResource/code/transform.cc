#include "transform.h"

namespace efiilj
{

	transform_model::transform_model(const vector3& pos, const vector3& rot, const vector3& scale)
	: position_(pos), scale_(scale), rotation_(rot), model_(true), is_changed_(true) { }
	
	const matrix4& transform_model::model()
	{
		if (is_changed_)
		{
			//TODO: Optimize transform model function.
			const matrix4 t = matrix4::get_translation(vector4(position_, 1));
			const matrix4 r = matrix4::get_rotation_xyz(rotation_);
			const matrix4 s = matrix4::get_scale(scale_);

			model_ = t * r * s;
			is_changed_ = false;
		}
		
		return model_;
	}

	vector3 transform_model::forward() const
	{
		return vector3(
			cos(rotation_.x()) * cos(rotation_.y()),
			sin(rotation_.x()),
			cos(rotation_.x()) * sin(rotation_.y())).norm();
	}

	vector3 transform_model::backward() const
	{
		return forward() * -1;
	}

	vector3 transform_model::left() const
	{
		return vector3::cross(vector3(0, 1, 0), forward()).norm();
	}

	vector3 transform_model::right() const
	{
		return left() * -1;
	}

	vector3 transform_model::up() const
	{
		return vector3::cross(forward(), left());
	}

	vector3 transform_model::down() const
	{
		return up() * -1;
	}
}
