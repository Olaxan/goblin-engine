#include "bounds.h"
#include "ray.h"
#include "phys_mesh.h"
#include "transform.h"

#include <memory>

namespace efiilj
{
	class physics_data
	{
		private:
			std::shared_ptr<physics_mesh> _mesh;
			std::shared_ptr<transform_model> _transform;

		public:

			physics_data(std::shared_ptr<physics_mesh> mesh, std::shared_ptr<transform_model> transform);

			bounds get_bounds() const;
			bool point_inside_bounds(const vector3& point) const;
			bool ray_intersect_bounds(const ray& ray, vector3& hit) const;
			bool ray_intersect_triangle(const ray& ray, vector3& hit, vector3& norm) const;

	};
}
