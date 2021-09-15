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

	struct trace_hit
	{
		vector3 position;
		vector3 normal;
		mesh_id mesh;
		entity_id entity;
	};

	class collider_manager : public manager<collider_id> 
	{
		private:

			typedef std::pair<collider_id, float> point;

			struct point_comp
			{
			  bool operator () (const point& lhs, const point& rhs) const
			  { return lhs.second < rhs.second; }
			};	

			void check_axis_sweep(const std::set<point, point_comp>& points, std::map<collider_id, std::set<collider_id>>& hits) const;

			bool point_inside_bounds(collider_id idx, const vector3& point) const;
			bool ray_intersect_triangle(collider_id idx, mesh_id mid, const ray& ray, vector3& hit, vector3& norm) const;

			struct PhysicsData
			{
				std::vector<bounds> mesh_bounds;
				std::vector<bool> bounds_updated;
				std::vector<std::set<collider_id>> collisions;
			} _data;

			std::shared_ptr<mesh_server> _meshes;
			std::shared_ptr<mesh_manager> _mesh_instances;
			std::shared_ptr<transform_manager> _transforms;

		public:

			collider_manager();
			~collider_manager();

			void extend_defaults(collider_id) override;
			void on_register(std::shared_ptr<manager_host> host) override;

			void draw_gui() override;
			void draw_gui(collider_id) override;

			bool update_bounds(collider_id idx);
			void update_broad();
			void update();

			bool test_hit(const ray& ray, trace_hit& hit) const;
			bool test_hit(collider_id idx, const ray& ray, trace_hit& hit) const;

			bool test_broad(collider_id idx) const
			{
				return _data.collisions[idx].size() > 0;
			}

			const bounds& get_bounds(collider_id idx) const
			{
				return _data.mesh_bounds[idx];
			}
			
			bounds get_bounds_world(collider_id idx) const;


	};
}
