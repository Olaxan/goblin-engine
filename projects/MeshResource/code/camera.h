#pragma once

#include "matrix4.h"
#include "transform.h"

namespace efiilj
{
	class camera_model
	{
	private:

		Vector3 up_axis_;
		Matrix4 perspective_;
		transform_model transform_;

	public:

		camera_model(float fov, float aspect, float near, float far, const transform_model& pos, const Vector3& up);

		void transform(const transform_model& trans) { transform_ = trans; }
		transform_model& transform() { return this->transform_; }

		void up(const Vector3& xyz) { up_axis_ = xyz; }
		const Vector3& up() const { return this->up_axis_; }

		Matrix4 view() const;

		~camera_model()
		= default;
	};
}
