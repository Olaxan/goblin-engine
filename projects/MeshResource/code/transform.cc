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
}
