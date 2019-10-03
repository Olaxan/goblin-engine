#include "transform.h"

namespace efiilj
{
	transform_model::transform_model(const vector3& pos, const vector3& rot, const vector3& scale)
	: model_(true), has_changed_(true), position(pos), scale(scale), rotation(rot) { }

	const matrix4& transform_model::model()
	{
		if (has_changed_)
		{
			const matrix4 t = matrix4::get_translation(vector4(position, 1));
			const matrix4 r = matrix4::get_rotation_xyz(rotation);
			const matrix4 s = matrix4::get_scale(scale);

			model_ = t * r * s;
			has_changed_ = false;
		}

		return model_;
	}
}
