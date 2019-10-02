#pragma once

#include "matrix4.h"

namespace efiilj
{
	class transform_model
	{
	private:

		Matrix4 model_;
		bool is_dirty_;

		void set_dirty(const bool dirty = true) { this->is_dirty_ = dirty; }

	public:

		explicit transform_model(const Vector3& pos = Vector3(0, 0, 0), const Vector3& rot = Vector3(0, 0, 0),
		                         const Vector3& scale = Vector3(1, 1, 1));

		Vector3 position;
		Vector3 scale;
		Vector3 rotation;

		Vector3 forward() const
		{
			return Vector3 (
				cos(rotation.x()) * cos(rotation.y()),
				sin(rotation.x()),
				cos(rotation.x()) * sin(rotation.y()));
		}

		Vector3 left() const
		{
			
		}

		const Matrix4& model();

		~transform_model()
			= default;
	};
}
