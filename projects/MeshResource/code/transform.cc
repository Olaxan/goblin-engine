#include "transform.h"

namespace efiilj
{

	transform_model::transform_model(const vector3& pos, const vector3& rot, const vector3& scale)
	: position(pos), scale(scale), rotation(rot), model_(true) { }
	
	const matrix4& transform_model::model()
	{
		//TODO: Optimize transform model function.
		const matrix4 t = matrix4::get_translation(vector4(position, 1));
		const matrix4 r = matrix4::get_rotation_xyz(rotation);
		const matrix4 s = matrix4::get_scale(scale);

		model_ = t * r * s;
		
		return model_;
	}

	vector3 transform_model::forward() const
	{
		return vector3(
			cos(rotation.x()) * cos(rotation.y()),
			sin(rotation.x()),
			cos(rotation.x()) * sin(rotation.y())).norm();
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
