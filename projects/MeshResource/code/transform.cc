#include "transform.h"

namespace efiilj
{
	TransformModel::TransformModel(Vector3 pos, Vector3 rot, Vector3 scale) : position(pos), rotation(rot), scale(scale), model(true), is_dirty(true) { }
	
	const Matrix4& TransformModel::Model()
	{
		if (is_dirty)
		{
			Matrix4 t = Matrix4::getTranslation(Vector4(position, 1));
			Matrix4 r = Matrix4::getRotationXYZ(rotation);
			Matrix4 s = Matrix4::getScale(scale);

			model = t * r * s;
			is_dirty = false;
		}

		return model;
	}
}