#include "camera.h"

namespace efiilj
{
	camera_model::
	camera_model(const float fov, const float aspect, const float near, const float far, std::shared_ptr<transform_model>& trans_ptr, const Vector3& up)
	: up_axis_(up), transform_(trans_ptr)
	{
		perspective_ = Matrix4::getPerspective(fov, aspect, near, far);
	}

	Matrix4 camera_model::view() const
	{
		const Vector3 rot = transform_->rotation();

		const Vector3 dir(
			cos(rot.x()) * cos(rot.y()),
			sin(rot.x()),
			cos(rot.x()) * sin(rot.y()));

		return perspective_ * Matrix4::getLookat(transform_->position(), transform_->position() + dir, up_axis_);
	}
}
