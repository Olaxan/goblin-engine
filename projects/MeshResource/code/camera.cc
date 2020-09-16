#include "camera.h"

namespace efiilj
{
	camera_model::
	camera_model(const float fov, const float aspect, const float near, const float far, std::shared_ptr<transform_model>& trans_ptr, const vector3& up)
	: up_axis_(up), transform_(trans_ptr)
	{
		perspective_ = matrix4::get_perspective(fov, aspect, near, far);
		get_view();
	}

	const matrix4& camera_model::get_view()
	{
		const vector4 camera_pos = transform_->position;
		const vector4 camera_direction = transform_->backward();
		const vector4 camera_right = transform_->right();
		const vector4 camera_up = transform_->up();

		view_ = matrix4(camera_right, camera_up, camera_direction, vector4(), true);
		view_(12) = vector4::dot(camera_right, camera_pos) * -1;
		view_(13) = vector4::dot(camera_up, camera_pos) * -1;
		view_(14) = vector4::dot(camera_direction, camera_pos) * -1;

		return view_;
	}
}
