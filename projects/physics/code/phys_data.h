#pragma once

#include "bounds.h"
#include "ray.h"

#include "entity.h"
#include "manager.h"
#include "trfm_mgr.h"
#include "mesh_srv.h"
#include "mesh_mgr.h"

#include <memory>

namespace efiilj
{

	typedef int collider_id;

	class collider_manager : public manager<collider_id> 
	{
		private:

			struct PhysicsData
			{
				std::vector<bounds> mesh_bounds;
				std::vector<bool> bounds_updated;
			} _data;

			std::shared_ptr<mesh_server> _meshes;
			std::shared_ptr<mesh_manager> _mesh_instances;
			std::shared_ptr<transform_manager> _transforms;

			bool point_inside_bounds(collider_id idx, const vector3& point) const;
			bool ray_intersect_triangle(collider_id idx, mesh_id mid, const ray& ray, vector3& hit, vector3& norm) const;

		public:

			collider_manager();
			~collider_manager();

			void extend_defaults(collider_id) override;
			void on_register(std::shared_ptr<manager_host> host) override;

			bool update_bounds(collider_id idx);

			bool test_hit(const ray& ray, vector3& hit, vector3& norm) const;
			bool test_hit(collider_id idx, const ray& ray, vector3& hit, vector3& norm) const;

			void update()
			{
				for (auto& idx : _instances)
					update_bounds(idx);
			}

			const bounds& get_bounds(collider_id idx) const
			{
				return _data.mesh_bounds[idx];
			}


	};
}
