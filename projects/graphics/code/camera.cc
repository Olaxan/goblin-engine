#include "camera.h"

namespace efiilj
{
	camera_model::
	camera_model(float fov, unsigned width, unsigned height, float near, float far, std::shared_ptr<transform_model>& trans_ptr, const vector3& up)
	: up_axis_(up), 
	width_(static_cast<float>(width)), 
	height_(static_cast<float>(height)), 
	transform_(trans_ptr)
	{
		float aspect = width_ / height_;
		perspective_ = matrix4::get_perspective(fov, aspect, near, far);
		get_view();
	}

	const matrix4& camera_model::get_view()
	{
		const vector4 camera_pos = transform_->get_position();
		const vector4 camera_direction = transform_->backward();
		const vector4 camera_right = transform_->right();
		const vector4 camera_up = transform_->up();

		view_ = matrix4(camera_right, camera_up, camera_direction, vector4(), true);
		view_(12) = vector4::dot(camera_right, camera_pos) * -1;
		view_(13) = vector4::dot(camera_up, camera_pos) * -1;
		view_(14) = vector4::dot(camera_direction, camera_pos) * -1;

		return view_;
	}

	ray camera_model::raycast(const int mouse_x, const int mouse_y, const float len)
	{
		float x = (2.0f * mouse_x) / width_ - 1.0f;
		float y = 1.0f - (2.0f * mouse_y) / height_;
		float z = 1.0f;

		vector3 ray_nds(x, y, z);
		vector4 ray_clip(x, y, -z, 1.0);
		vector4 ray_eye = perspective_.inverse() * ray_clip;
		ray_eye = vector4(ray_eye.x(), ray_eye.y(), -z, 0.0f);

		vector4 ray_world = view_.inverse() * ray_eye;
		ray_world = ray_world.norm();

		return ray(transform_->get_position(), ray_world * len);
	}
}
