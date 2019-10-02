#include "transform.h"

namespace efiilj
{
	transform_model::transform_model(const Vector3& pos, const Vector3& rot, const Vector3& scale)
	: model_(true), is_dirty_(true), position(pos), scale(scale), rotation(rot) { }

	const Matrix4& transform_model::model()
	{
		if (is_dirty_)
		{
			const Matrix4 t = Matrix4::get_translation(Vector4(position, 1));
			const Matrix4 r = Matrix4::get_rotation_xyz(rotation);
			const Matrix4 s = Matrix4::get_scale(scale);

			model_ = t * r * s;
			is_dirty_ = false;
		}

		return model_;
	}
}
