#include "camera.h"

namespace efiilj
{
	camera_model::
	camera_model(const float fov, const float aspect, const float near, const float far, std::shared_ptr<transform_model>& trans_ptr, const vector3& up)
	: up_axis_(up), transform_(trans_ptr)
	{
		perspective_ = matrix4::getPerspective(fov, aspect, near, far);
	}

	matrix4 camera_model::view() const
	{

		const vector3 camera_pos = transform_->position();
		const vector3 camera_direction = transform_->backward();
		const vector3 camera_right = transform_->right();
		const vector3 camera_up = transform_->up();

		matrix4 a = matrix4(vector4(camera_right, 1), vector4(camera_up, 1), vector4(camera_direction, 1), vector4());
		a(3, 0) = vector3::dot(camera_right, camera_pos) * -1;
		a(3, 1) = vector3::dot(camera_up, camera_pos) * -1;
		a(3, 2) = vector3::dot(camera_direction, camera_pos) * -1;

		return perspective_ * a;
	}
}
