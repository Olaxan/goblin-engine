#include "transform.h"

namespace efiilj
{
	transform_model::transform_model(const Vector3& pos, const Vector3& rot, const Vector3& scale) : position_(pos), scale_(scale),
	                                                                          rotation_(rot), model_(true), is_dirty_(true)
	{
	}

	const Matrix4& transform_model::model()
	{
		if (is_dirty_)
		{
			const Matrix4 t = Matrix4::getTranslation(Vector4(position_, 1));
			const Matrix4 r = Matrix4::getRotationXYZ(rotation_);
			const Matrix4 s = Matrix4::getScale(scale_);

			model_ = t * r * s;
			is_dirty_ = false;
		}

		return model_;
	}
}
