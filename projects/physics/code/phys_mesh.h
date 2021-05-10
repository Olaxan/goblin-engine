#pragma once

#include "vector3.h"
#include "bounds.h"

#include <vector>

namespace efiilj
{
	class physics_mesh
	{
		private:

		std::vector<vector3> _positions;
		std::vector<vector3> _normals;
		std::vector<unsigned> _indices;

		bounds _bounds;

		public:

		void set_bounds(const vector3& min, const vector3& max) { _bounds = bounds(min,  max); }
		void set_bounds(const bounds& bounds) { _bounds = bounds; }
		const bounds& get_bounds() { return _bounds; }
		bounds get_bounds(const matrix4& mat) { return _bounds.get_transformed_bounds(mat); }

		friend class gltf_model_loader;
		friend class physics_data;

	};
}
