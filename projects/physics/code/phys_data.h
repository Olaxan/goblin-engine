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
		collider_id collider;
		entity_id entity;
	};

	struct SupportPoint
	{
		vector3 s1;
		vector3 s2;
		vector3 point;

		SupportPoint()
			: s1(vector3()), s2(vector3()), point(vector3()) { }

		SupportPoint(const vector3& a, const vector3& b)
			: s1(a), s2(b), point(a - b) { }

		bool operator == (const SupportPoint& other)
		{
			return point == other.point;
		}
	};

	struct SupportEdge
	{
		SupportPoint a;
		SupportPoint b;

		SupportEdge()
			: a(SupportPoint()), b(SupportPoint()) {}

		SupportEdge(const SupportPoint& a, const SupportPoint& b)
			: a(a), b(b) {}
	};

	struct SupportFace
	{
		SupportPoint a;
		SupportPoint b;
		SupportPoint c;
		vector3 normal;

		SupportFace()
			: a(SupportPoint()), b(SupportPoint()), c(SupportPoint()),
			normal(vector3()) {}

		SupportFace(
				const SupportPoint& a, 
				const SupportPoint& b, 
				const SupportPoint& c
			) 
			: a(a), b(b), c(c), 
			normal(vector3::cross(b.point - a.point, c.point - a.point).norm()) {}
	};

	struct Simplex
	{
		int dim;

		SupportPoint points[4];	
		SupportPoint& a;
		SupportPoint& b;
		SupportPoint& c;
		SupportPoint& d;

		Simplex()
			  : dim(0),
			  a(points[0]),
			  b(points[1]),
			  c(points[2]),
			  d(points[3]) { }

		void clear() { dim = 0; }

		void set(SupportPoint a, SupportPoint b, SupportPoint c, SupportPoint d)
		{
			dim = 4;
			this->a = a;
			this->b = b;
			this->c = c;
			this->d = d;
		}

		void set(SupportPoint a, SupportPoint b, SupportPoint c)
		{
			dim = 3;
			this->a = a;
			this->b = b;
			this->c = c;
		}

		void set(SupportPoint a, const SupportPoint& b)
		{
			dim = 2;
			this->a = a;
			this->b = b;
		}

		void set(SupportPoint a)
		{
			dim = 1;
			this->a = a;
		}

		void push(SupportPoint p)
		{
			dim = std::min(dim + 1, 4);
			for (int i = dim - 1; i > 0; i--)
				points[i] = points[i - 1];

			points[0] = p;
		}

	};

	struct Collision
	{
		collider_id object1;
		collider_id object2;
		vector3 point1;
		vector3 point2;
		vector3 normal;
		float depth;

		SupportFace face;

		Collision() 
			: object1(-1),
			object2(-1),
			point1(vector3()),
			point2(vector3()),
			normal(vector3()),
			depth(0) {}

		Collision(
				collider_id obj1, 
				collider_id obj2,
				const vector3& point1,
				const vector3& point2,
				const vector3& normal,
				float depth)
			: 
				object1(obj1), object2(obj2), 
				point1(point1), point2(point2),
				normal(normal), depth(depth) { }
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

			std::set<point, point_comp> _sweep_x;
			std::set<point, point_comp> _sweep_y;
			std::set<point, point_comp> _sweep_z;

			std::map<collider_id, std::set<collider_id>> _sweep_hits_x;
			std::map<collider_id, std::set<collider_id>> _sweep_hits_y;
			std::map<collider_id, std::set<collider_id>> _sweep_hits_z;

			struct PhysicsData
			{
				std::vector<bounds> mesh_bounds;
				std::vector<std::set<collider_id>> broad_collisions;
				std::vector<std::set<collider_id>> narrow_collisions;
				std::vector<std::vector<Collision>> collisions;
			} _data;

			std::shared_ptr<mesh_server> _meshes;
			std::shared_ptr<mesh_manager> _mesh_instances;
			std::shared_ptr<transform_manager> _transforms;

			void clear_sweep();
			void check_axis_sweep(const std::set<point, point_comp>& points, std::map<collider_id, std::set<collider_id>>& hits) const;

			bool update_simplex(SupportPoint simplex[4], int& dim, vector3& dir) const;
			bool gjk(collider_id col1, collider_id col2, Simplex& simplex) const;
			bool epa(collider_id col1, collider_id col2, const Simplex& simplex, Collision& result) const;

			bool point_inside_bounds(collider_id idx, const vector3& point) const;
			bool ray_intersect_triangle(collider_id idx, mesh_id mid, const ray& ray, vector3& hit, vector3& norm) const;

		public:

			collider_manager();
			~collider_manager();

			SupportPoint support(collider_id, collider_id, const vector3& dir) const;
			vector3 get_furthest_point(collider_id, const vector3& dir) const;

			void extend_defaults(collider_id) override;
			void on_register(std::shared_ptr<manager_host> host) override;

			void on_editor_gui() override;
			void on_editor_gui(collider_id) override;

			void on_activate(collider_id idx) override;
			void on_begin_frame() override;

			bool update_bounds(collider_id idx);
			void update_broad();
			void update_narrow();

			void test_scene();

			bool test_hit(const ray& ray, trace_hit& hit) const;
			bool test_hit(collider_id idx, const ray& ray, trace_hit& hit) const;
			bool test_collision(collider_id obj1, collider_id obj2, Collision& col1, Collision& col2) const;

			bool test_broad(collider_id idx) const
			{ 
				return _data.broad_collisions[idx].size() > 0; 
			}

			bool test_broad(collider_id obj1, collider_id obj2) const
			{
				return _data.broad_collisions[obj1].find(obj2) != _data.broad_collisions[obj1].end();
			}

			bool test_narrow(collider_id idx) const
			{ 
				return _data.narrow_collisions[idx].size() > 0; 
			}

			bool test_narrow(collider_id obj1, collider_id obj2) const
			{
				return _data.narrow_collisions[obj1].find(obj2) != _data.narrow_collisions[obj1].end();
			}

			bool collides_with(collider_id idx, collider_id other)
			{
				return _data.narrow_collisions[idx].find(other) !=
					_data.narrow_collisions[idx].end();
			}

			const std::vector<Collision>& get_collisions(collider_id idx)
			{
				return _data.collisions[idx];
			}

			const bounds& get_bounds(collider_id idx) const
			{ return _data.mesh_bounds[idx]; }
			
			bounds get_bounds_world(collider_id idx) const;


	};
}
