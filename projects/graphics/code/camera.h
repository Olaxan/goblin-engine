#pragma once

#include "matrix4.h"
#include "transform.h"
#include "ray.h"

#include <memory>

namespace efiilj
{
	/**
	 * \brief Class to represent a camera in 3D space.
	 */
	class camera_model
	{
	private:

		vector3 up_axis_;
		matrix4 perspective_;
		matrix4 perspective_inverse_;
		matrix4 view_;

		float width_, height_;

		std::shared_ptr<transform_model> transform_;

	public:

		/**
		 * \brief Creates a new camera instance.
		 * \param fov Camera field of view in radians
		 * \param aspect Camera width/height aspect ration
		 * \param near Near clipping plane
		 * \param far Far clipping plane
		 * \param trans_ptr Pointer to the transform object moving this camera
		 * \param up The vector representing up in this world space
		 */
		camera_model(float fov, unsigned width, unsigned height, float near, float far, std::shared_ptr<transform_model>& trans_ptr, const vector3& up);

		camera_model(camera_model& copy)
			= default;

		camera_model(camera_model&& move)
			= default;

		std::shared_ptr<transform_model> get_transform() const { return transform_; }
		void set_transform(std::shared_ptr<transform_model>& trans) { transform_ = std::move(trans); }

		const vector3& get_up() const { return this->up_axis_; }
		void set_up(const vector3& xyz) { up_axis_ = xyz; }
		
		vector3 camera_right() const { return this->view_.col(0).xyz(); }
		vector3 camera_left() const { return -camera_right(); }

		/**
		 * \brief Returns the perspective matrix for the camera.
		 * \return A 4D matrix for perspective calculations.
		 */
		const matrix4& get_perspective() const { return perspective_; }

		/**
		 * \brief Builds a view matrix with the current values and returns it.
		 * \return A 4-dimensional matrix for view projection of a vertex in 3D space
		 */
		const matrix4& get_view();

		ray get_ray_from_camera(const int x, const int y);

		~camera_model()
			= default;
	};
}
