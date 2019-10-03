#pragma once

#include "matrix4.h"

namespace efiilj
{
	struct transform_model
	{
	private:

		matrix4 model_;
		bool has_changed_;

	public:

		explicit transform_model(const vector3& pos = vector3(0, 0, 0), const vector3& rot = vector3(0, 0, 0),
		                         const vector3& scale = vector3(1, 1, 1));

		vector3 position;
		vector3 scale;
		vector3 rotation;

		vector3 forward() const
		{
			return vector3 (
				cos(rotation.x()) * cos(rotation.y()),
				sin(rotation.x()),
				cos(rotation.x()) * sin(rotation.y()));
		}

		vector3 left() const
		{
			
		}

		const matrix4& model();

		~transform_model()
			= default;
	};
}
