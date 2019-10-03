#pragma once

#include "matrix4.h"
#include "transform.h"

#include <memory>

namespace efiilj
{
	class camera_model
	{
	private:

		vector3 up_axis_;
		matrix4 perspective_;
		std::shared_ptr<transform_model> transform_;

	public:

		camera_model(float fov, float aspect, float near, float far, std::shared_ptr<transform_model>& trans_ptr, const vector3& up);

		const transform_model& transform() const { return *this->transform_; }
		//void transform(const transform_model& trans) { transform_ = trans; }

		const vector3& up() const { return this->up_axis_; }
		void up(const vector3& xyz) { up_axis_ = xyz; }

		matrix4 view() const;

		~camera_model()
		= default;
	};
}
