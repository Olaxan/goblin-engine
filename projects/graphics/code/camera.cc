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
		perspective_inverse_ = perspective_.inverse();
		get_view();
	}

	const matrix4& camera_model::get_view()
	{
		view_ = matrix4::get_lookat(
					transform_->get_position(),
					transform_->get_position() + transform_->forward(),
					vector4(0, 1.0f, 0, 0)
				);

		return view_;
	}

	ray camera_model::get_ray_from_camera(const int mouse_x, const int mouse_y)
	{
		float x = (2.0f * mouse_x) / width_ - 1.0f;
		float y = 1.0f - (2.0f * mouse_y) / height_;

		printf("NDC X %f, %f\n", x, y);

		vector4 ray_clip(x, y, -1.0f, 1.0f);
		vector4 ray_eye = perspective_inverse_ * ray_clip;
		ray_eye = vector4(ray_eye.x(), ray_eye.y(), -1.0f, 0.0f);

		vector4 ray_world = view_.inverse() * ray_eye;
		vector3 fuck_you(ray_world.x(), ray_world.y(), ray_world.z());
		fuck_you = fuck_you.norm();

		return ray(transform_->get_position(), vector4(fuck_you, 1.0f));
	}
}
