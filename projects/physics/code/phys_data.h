#include "bounds.h"
#include "ray.h"
#include "transform.h"
#include "node.h"

#include <memory>

namespace efiilj
{
	class physics_node
	{
		private:

			std::shared_ptr<graphics_node> _node;

			mesh_resource& _mesh;
			transform_model& _transform;

		public:

			physics_node(std::shared_ptr<graphics_node> node);

			bounds get_bounds() const;
			bool point_inside_bounds(const vector3& point) const;
			bool ray_intersect_bounds(const ray& ray, vector3& hit) const;
			bool ray_intersect_triangle(const ray& ray, vector3& hit, vector3& norm) const;

	};
}
