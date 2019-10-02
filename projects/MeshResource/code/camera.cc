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
		return perspective_ * Matrix4::get_lookat(transform_->position, transform_->position + transform_->forward(), up_axis_);
	}
}
