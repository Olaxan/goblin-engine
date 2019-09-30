#include "camera.h"

namespace efiilj
{
	CameraModel::CameraModel(float fov, float aspect, float near, float far, TransformModel pos, Vector3 up) : transform(pos), up_axis(up)
	{
		perspective = Matrix4::getPerspective(fov, aspect, near, far);
	}

	Matrix4 CameraModel::View()
	{
		Vector3 rot = transform.Rotation();

		Vector3 dir(
			cos(rot.x()) * cos(rot.y()), 
			sin(rot.x()),
			cos(rot.x()) * sin(rot.y()));

		return perspective * Matrix4::getLookat(transform.Position(), transform.Position() + dir, up_axis);
	}
}