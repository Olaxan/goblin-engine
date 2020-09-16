#include "transform.h"

namespace efiilj
{

	transform_model::transform_model(const vector3& pos, const vector3& rot, const vector3& scale)
	: model_(true), position(pos, 1), scale(scale, 1), rotation(rot, 1) { }
	
	const matrix4& transform_model::model()
	{
		//TODO: Optimize transform model function.
		const matrix4 t = matrix4::get_translation(position);
		const matrix4 r = matrix4::get_rotation_xyz(rotation);
		const matrix4 s = matrix4::get_scale(scale);

		model_ = t * r * s;
		
		return model_;
	}

	const matrix4& transform_model::model_inv()
	{
		inverse_ = model_.inverse();
		return inverse_;
	}

	vector4 transform_model::forward() const
	{
		return vector4(
			cos(rotation.x()) * cos(rotation.z()),
			sin(rotation.x()),
			cos(rotation.x()) * sin(rotation.z()), 1).norm();
	}

	vector4 transform_model::backward() const
	{
		return forward() * -1;
	}

	vector4 transform_model::left() const
	{
		return vector4::cross(vector4(0, 1, 0, 0), forward()).norm();
	}

	vector4 transform_model::right() const
	{
		return left() * -1;
	}

	vector4 transform_model::up() const
	{
		return vector4::cross(forward(), left());
	}

	vector4 transform_model::down() const
	{
		return up() * -1;
	}
}
