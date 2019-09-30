#pragma once

#include "matrix4.h"
#include "transform.h"

namespace efiilj
{
	class CameraModel
	{
	private:

		Vector3 up_axis;
		Matrix4 perspective;
		TransformModel transform;

	public:

		CameraModel(float fov, float aspect, float near, float far, TransformModel pos, Vector3 up);

		void Transform(const TransformModel& trans) { transform = trans; }
		TransformModel& Transform() { return this->transform; }

		void Up(const Vector3& xyz) { up_axis = xyz; }
		const Vector3& Up() const { return this->up_axis; }

		Matrix4 View();

		~CameraModel() { }
	};
}