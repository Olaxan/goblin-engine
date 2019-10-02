#pragma once

#include "matrix4.h"
#include "transform.h"

#include <memory>

namespace efiilj
{
	class camera_model
	{
	private:

		Vector3 up_axis_;
		Matrix4 perspective_;
		std::shared_ptr<transform_model> transform_;

	public:

		camera_model(float fov, float aspect, float near, float far, std::shared_ptr<transform_model>& trans_ptr, const Vector3& up);

		const transform_model& transform() const { return *this->transform_; }
		//void transform(const transform_model& trans) { transform_ = trans; }

		const Vector3& up() const { return this->up_axis_; }
		void up(const Vector3& xyz) { up_axis_ = xyz; }

		Matrix4 view() const;

		~camera_model()
		= default;
	};
}
