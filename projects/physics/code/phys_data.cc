#include "phys_data.h"
#include "imgui.h"

#include <algorithm>
#include <iterator>
#include <memory>
#include <limits>
#include <tuple>

#define IS_ALIGNED(a, b) (vector3::dot(a, b) > 0)
#define IS_NOT_ALIGNED(a, b) (vector3::dot(a, b) < 0)

namespace efiilj
{

	collider_manager::collider_manager()
	{
		printf("Init collider...\n");
		_name = "Mesh collider";
	}

	collider_manager::~collider_manager()
	{
		printf("Collider manager exit\n");
	}

	void collider_manager::extend_defaults(collider_id idx)
	{
		_data.mesh_bounds.emplace_back();
		_data.broad_collisions.emplace_back();
		_data.narrow_collisions.emplace_back();
		_data.collisions.emplace_back();
	}

	void collider_manager::on_register(std::shared_ptr<manager_host> host)
	{
		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');
		_mesh_instances = host->get_manager_from_fcc<mesh_manager>('MEMR');
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
	}

	void collider_manager::on_activate(collider_id idx)
	{
		update_bounds(idx);
	}

	void collider_manager::on_begin_frame()
	{
		//test_scene();
	}

	void collider_manager::on_editor_gui()
	{}

	void collider_manager::on_editor_gui(collider_id idx)
	{

		ImGui::Text("Collider %d", idx);

		if (ImGui::Button("Recalculate AABB"))
			update_bounds(idx);

		std::stringstream ss;

		for (auto col : _data.broad_collisions[idx])
			ss << col << ", ";

		ImGui::Text("Broad: %s", ss.str().c_str());

		ss.clear();

		for (auto col : _data.narrow_collisions[idx])
			ss << col << ", ";

		ImGui::Text("Narrow: %s", ss.str().c_str());

		entity_id eid = get_entity(idx);
		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return;

		ImVec4 yellow(1, 1, 0, 1);

		for (const auto& col : _data.collisions[idx])
		{
			ImGui::TextColored(yellow, "Collision --> %d", col.object1);

			const vector3& n = col.normal;
			const vector3& p1 = col.point1;
			const vector3& p2 = col.point2;

			ImGui::Text("Normal: %f %f %f", n.x, n.y, n.z);
			ImGui::Text("Point 1: %f %f %f", p1.x, p1.y, p1.z);
			ImGui::Text("Point 2: %f %f %f", p2.x, p2.y, p2.z);
			ImGui::Text("Depth: %f", col.depth);

			if (ImGui::SmallButton("Free"))
				_transforms->add_position(trf_id, col.normal * col.depth);

			ImGui::SameLine();

			if (ImGui::SmallButton("To Collision"))
				_transforms->set_position(trf_id, col.point1);
		}
	}

	bool collider_manager::update_bounds(collider_id idx)
	{

		entity_id eid = _entities[idx];

		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return false;

		vector3 min(
				std::numeric_limits<float>::max(),
				std::numeric_limits<float>::max(),
				std::numeric_limits<float>::max()
				);

		vector3 max(
				std::numeric_limits<float>::min(),
				std::numeric_limits<float>::min(),
				std::numeric_limits<float>::min()
			 );

		auto range = _mesh_instances->get_components(eid);

		for (auto it = range.first; it != range.second; it++)
		{
			mesh_id mid = _mesh_instances->get_mesh(it->second);

			assert(("BOUNDS: Invalid mesh id!", _meshes->is_valid(mid)));

			min = vector3::min(min, _meshes->get_min(mid));
			max = vector3::max(max, _meshes->get_max(mid));
		}

		_data.mesh_bounds[idx] = bounds(min, max);

		return true;

	}

	bounds collider_manager::get_bounds_world(collider_id idx) const
	{
		const bounds& b = get_bounds(idx);

		entity_id eid = get_entity(idx);

		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return b;

		const matrix4& model = _transforms->get_model(trf_id);

		return b.get_transformed_bounds(model);
	}
	
	void collider_manager::clear_sweep()
	{
		_sweep_x.clear();
		_sweep_y.clear();
		_sweep_z.clear();

		_sweep_hits_x.clear();
		_sweep_hits_y.clear();
		_sweep_hits_z.clear();
	}

	void collider_manager::check_axis_sweep(const std::set<point, point_comp>& points, std::map<collider_id, std::set<collider_id>>& hits) const
	{

		std::set<collider_id> intervals;

		for (const auto& point : points)
		{
			collider_id new_id = point.first;

			// If this is the beginning of a new interval
			if (intervals.find(new_id) == intervals.end())
			{
				// Add the new interval id to all existing intervals
				for (const auto& idx : intervals)
					hits[idx].insert(new_id);

				// Add the existing intervals to the new interval
				hits[new_id].insert(intervals.begin(), intervals.end());

				// Insert the new interval
				intervals.insert(new_id);
			}
			else
			{
				// Otherwise erase the interval
				intervals.erase(new_id);
			}
		}
	}

	void collider_manager::update_broad()
	{

		clear_sweep();

		for (const auto& idx : _instances)
		{
			const auto& bounds = get_bounds_world(idx);

			_sweep_x.emplace(idx, bounds.min.x);
			_sweep_x.emplace(idx, bounds.max.x);

			_sweep_y.emplace(idx, bounds.min.y);
			_sweep_y.emplace(idx, bounds.max.y);

			_sweep_z.emplace(idx, bounds.min.z);
			_sweep_z.emplace(idx, bounds.max.z);

			_data.broad_collisions[idx].clear();
		}

		check_axis_sweep(_sweep_x, _sweep_hits_x);
		check_axis_sweep(_sweep_y, _sweep_hits_y);
		check_axis_sweep(_sweep_z, _sweep_hits_z);

		for (const auto& idx : _instances)
		{
			std::set<collider_id> s1;
			std::set<collider_id> s2;

			std::set_intersection(
					_sweep_hits_x[idx].begin(), _sweep_hits_x[idx].end(), 
					_sweep_hits_y[idx].begin(), _sweep_hits_y[idx].end(),
					std::inserter(s1, s1.begin()));

			std::set_intersection(
					s1.begin(), s1.end(), 
					_sweep_hits_z[idx].begin(), _sweep_hits_z[idx].end(),
					std::inserter(s2, s2.begin()));

			_data.broad_collisions[idx] = s2;
		}
	}

	SupportPoint collider_manager::support(collider_id col1, collider_id col2, const vector3& dir) const
	{
		vector3 a = get_furthest_point(col1, dir);
		vector3 b = get_furthest_point(col2, -dir);

		return SupportPoint(a, b);
	}

	vector3 collider_manager::get_furthest_point(collider_id idx, const vector3& dir) const
	{
		entity_id eid = get_entity(idx);

		transform_id trf_id = _transforms->get_component(eid);

		assert(("Invalid transform id, get_furthest_point!", //NOLINT
					_transforms->is_valid(trf_id)));

		vector3 pos = _transforms->get_position(trf_id);
		vector3 inv_dir = _transforms->get_model_inv(trf_id) * (dir + pos);
		vector3 furthest_point = vector3();
		float max_dot = std::numeric_limits<float>::min();

		auto range = _mesh_instances->get_components(eid);

		for (auto it = range.first; it != range.second; it++)
		{
			mesh_instance_id miid = it->second;
			mesh_id mid = _mesh_instances->get_mesh(miid);

			const auto& points = _meshes->get_positions(mid);

			for (const auto& point : points)
			{
				float d = vector3::dot(point, inv_dir);

				if (d > max_dot)
				{
					max_dot = d;
					furthest_point = point;
				}
			}
		}
		
		return _transforms->get_model(trf_id) * furthest_point;
	}

	inline vector3 cross_aba(const vector3& a, const vector3& b)
	{
		return vector3::cross(vector3::cross(a, b), a);
	}

#define EPA_MAX_ITERATIONS 32
#define EPA_TOLERANCE 0.0001f

	void barycentric(const vector3& p, const vector3& a, const vector3& b, const vector3& c, float* u, float* v, float* w) 
	{
		// code from Crister Erickson's Real-Time Collision Detection
		vector3 v0 = b - a,v1 = c - a,v2 = p - a;
		float d00 = vector3::dot(v0,  v0);
		float d01 = vector3::dot(v0,  v1);
		float d11 = vector3::dot(v1,  v1);
		float d20 = vector3::dot(v2,  v0);
		float d21 = vector3::dot(v2,  v1);
		float denom = d00 * d11 - d01 * d01;
		*v = (d11 * d20 - d01 * d21) / denom;
		*w = (d00 * d21 - d01 * d20) / denom;
		*u = 1.0f - *v - *w;
	}

	bool is_valid(const float& c)
	{
		return (c == c) && (c > 0.0f && c < 1.0f);
	}

	bool collider_manager::epa(collider_id col1, collider_id col2, const Simplex& simplex, Collision& result) const
	{
		const SupportPoint& a = simplex.a;	
		const SupportPoint& b = simplex.b;	
		const SupportPoint& c = simplex.c;	
		const SupportPoint& d = simplex.d;	

		result.object1 = col1;
		result.object2 = col2;

		std::vector<SupportFace> faces;
		std::vector<SupportEdge> edges;

		// Add termination simplex to face vector
		faces.emplace_back(a, b, c);
		faces.emplace_back(a, c, d);
		faces.emplace_back(a, d, b);
		faces.emplace_back(b, d, c);

		for (size_t iterations = 0; iterations < EPA_MAX_ITERATIONS; iterations++)
		{
			auto closest_face = faces.begin();
			float min_dist = std::numeric_limits<float>::max();

			for (auto it = faces.begin(); it != faces.end(); it++)
			{
				float dist = fabs(vector3::dot(it->a.point, it->normal));

				if (dist < min_dist)
				{
					min_dist = dist;
					closest_face = it;
				}
			}

			vector3 search_dir = closest_face->normal;

			SupportPoint sup = support(col1, col2, search_dir);

			float d = vector3::dot(sup.point, search_dir);

			if (d - min_dist < EPA_TOLERANCE)
			{
				float bary_u, bary_v, bary_w;

     			barycentric(closest_face->normal * min_dist,
                    closest_face->a.point,
                    closest_face->b.point,
                    closest_face->c.point,
                    &bary_u,
                    &bary_v,
                    &bary_w);

				result.normal = closest_face->normal;
				result.depth = min_dist;
				result.face = *closest_face;

				result.point1 = bary_u * closest_face->a.s1 
					+ bary_v * closest_face->b.s1 
					+ bary_w * closest_face->c.s1;

				result.point2 = bary_u * closest_face->a.s2
					+ bary_v * closest_face->b.s2
					+ bary_w * closest_face->c.s2;

				assert(("EPA: Zero normal", 
							!closest_face->normal.is_zero()));

				assert(("EPA: Invalid barycentric!",
							is_valid(bary_u) && is_valid(bary_v) && is_valid(bary_w)));

				assert(("EPA: Mismatched barycentric!",
							is_valid(bary_u + bary_v + bary_w)));

				return true;
			}

			edges.clear();

			const auto add_edge = [&](const SupportPoint& a, const SupportPoint& b)
			{
				for (auto it = edges.begin(); it != edges.end(); it++)
				{
					if (it->a == b && it->b == a)
					{
						edges.erase(it);
						return;
					}
				}
				edges.emplace_back(a, b);
			};

			for (auto it = faces.begin(); it != faces.end();)
			{
				if (IS_ALIGNED(it->normal, sup.point - it->a.point))
				{
					add_edge(it->a, it->b);
					add_edge(it->b, it->c);
					add_edge(it->c, it->a);
					it = faces.erase(it);
					continue;
				}
				it++;
			}

			for (auto it = edges.begin(); it != edges.end(); it++)
			{
				faces.emplace_back(sup, it->a, it->b);
			}
		}

		return false;
	}

#define GJK_MAX_ITERATIONS 64
	
	bool collider_manager::gjk(collider_id col1, collider_id col2, Simplex& simplex) const
	{

		const vector3& a = simplex.a.point;
		const vector3& b = simplex.b.point;
		const vector3& c = simplex.c.point;
		const vector3& d = simplex.d.point;

		simplex.clear();

		vector3 search_dir(1, 0, 0);
		SupportPoint s = support(col1, col2, search_dir);

		if (fabs(vector3::dot(search_dir, s.point)) 
				>= s.point.magnitude() * 0.8f)
		{
			search_dir = vector3(0, 1, 0);
			s = support(col1, col2, search_dir);
		}

		simplex.push(s);
		search_dir = -s.point;

		for (size_t iterations = 0; iterations < GJK_MAX_ITERATIONS; iterations++)
		{
			assert(("GJK: Search direction is zero vector", 
						!search_dir.is_zero()));

			SupportPoint p = support(col1, col2, search_dir);

			if (IS_NOT_ALIGNED(p.point, search_dir))
				return false;

			simplex.push(p);

			const vector3 ao = -a;

			switch (simplex.dim)
			{
				case 2:
				{
					const vector3 ab = b - a;
					search_dir = cross_aba(ab, ao);
					continue;
				}
				case 3:
				{ 
					const vector3 ab = b - a;
					const vector3 ac = c - a;
					const vector3 ad = d - a;
					const vector3 abc = vector3::cross(ab, ac);

					if (IS_ALIGNED(vector3::cross(ab, abc), ao))
					{
						simplex.set(simplex.a, simplex.b);
						search_dir = cross_aba(ab, ao);
						continue;
					}

					if (IS_ALIGNED(vector3::cross(abc, ac), ao))
					{
						simplex.set(simplex.a, simplex.c);
						search_dir = cross_aba(ac, ao);
						continue;
					}

					if (IS_ALIGNED(abc, ao))
					{
						search_dir = abc;
						continue;
					}

					simplex.set(simplex.a, simplex.c, simplex.b);
					search_dir = -abc;
					continue;
				}
				case 4:
				{
					{
						const vector3 ab = b - a;
						const vector3 ac = c - a;

						if (IS_ALIGNED(vector3::cross(ab, ac), ao))
							goto check_face;

						const vector3 ad = d - a;

						if (IS_ALIGNED(vector3::cross(ac, ad), ao))
						{
							simplex.set(simplex.a, simplex.c, simplex.d);
							goto check_face;
						}

						if (IS_ALIGNED(vector3::cross(ad, ab), ao))
						{
							simplex.set(simplex.a, simplex.d, simplex.b);
							goto check_face;
						}

						return true;
					}

check_face:

					const vector3 ab = b - a;
					const vector3 ac = c - a;
					const vector3 abc = vector3::cross(ab, ac);

					if (IS_ALIGNED(vector3::cross(ab, abc), ao))
					{
						simplex.set(simplex.a, simplex.b);
						search_dir = cross_aba(ab, ao);
						continue;
					}

					if (IS_ALIGNED(vector3::cross(abc, ac), ao))
					{
						simplex.set(simplex.a, simplex.c);
						search_dir = cross_aba(ac, ao);
						continue;
					}

					simplex.set(simplex.a, simplex.b, simplex.c);
					search_dir = abc;
					continue;

				}
			}
		}

		return false;
	}
		
	void collider_manager::update_narrow()
	{

		for (auto idx : _instances)
		{
			_data.narrow_collisions[idx].clear();
			_data.collisions[idx].clear();
		}

		for (auto idx : _instances)
		{
			//for (auto col : _instances)
			for (auto col : _data.broad_collisions[idx])
			{

				if (col == idx)
					continue;

				if (collides_with(idx, col))
					continue;

				Collision col1, col2;
				if (test_collision(idx, col, col1, col2))
				{
					_data.narrow_collisions[idx].insert(col);
					_data.narrow_collisions[col].insert(idx);

					_data.collisions[idx].emplace_back(col2);
					_data.collisions[col].emplace_back(col1);
				}
			}
		}
	}

	void collider_manager::test_scene()
	{
		for (auto& idx : _instances)
		{
			update_bounds(idx);
		}

		update_broad();
		update_narrow();
	}

	bool collider_manager::test_collision(collider_id obj1, collider_id obj2, Collision& col1, Collision& col2) const
	{

		Simplex simplex;

		if (gjk(obj1, obj2, simplex))
		{
			assert(("GJK / EPA: Collision disagreement!", 
						epa(obj1, obj2, simplex, col1) && epa(obj2, obj1, simplex, col2)));

			return true;
		}

		return false;
	}

	bool collider_manager::test_hit(const ray& ray, trace_hit& result) const
	{

		float nearest = std::numeric_limits<float>::max();

		bool ret = false;
		trace_hit temp_result;

		for (const auto& idx : _instances)
		{
			if (test_hit(idx, ray, temp_result))
			{
				ret = true;
				float len = (temp_result.position - ray.origin).length();
				if (len < nearest)
				{
					nearest = len;
					result = temp_result;
				}
			}
		}

		return ret;
	}

	bool collider_manager::test_hit(collider_id idx, const ray& ray, trace_hit& result) const
	{

		bool ret = false;
		float nearest = std::numeric_limits<float>::max();
		vector3 near_hit;
		vector3 near_norm;

		entity_id eid = _entities[idx];

		if (!get_bounds_world(idx).ray_intersection(ray, near_hit))
			return false;

		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return false;

		const matrix4& model = _transforms->get_model(trf_id);

		auto range = _mesh_instances->get_components(eid);
		for (auto it = range.first; it != range.second; it++)
		{
			mesh_id mid = _mesh_instances->get_mesh(it->second);

			bounds b = _meshes->get_bounds(mid);
			bounds bt = b.get_transformed_bounds(model);

			if (!bt.ray_intersection(ray, near_hit))
				continue;

			if (ray_intersect_triangle(idx, mid, ray, near_hit, near_norm))
			{
				ret = true;
				float len = (near_hit - ray.origin).length();
				if (len < nearest)
				{
					nearest = len;
					result.position = near_hit;
					result.normal = near_norm;
					result.mesh = mid;
					result.collider = idx;
					result.entity = eid;
				}
			}
		}

		return ret;
	}

	bool collider_manager::point_inside_bounds(collider_id idx, const vector3& point) const
	{
		return _data.mesh_bounds[idx].point_inside(point);
	}

	bool collider_manager::ray_intersect_triangle(collider_id idx, mesh_id mid, const ray& test, vector3& hit, vector3& norm) const
	{

		transform_id trf_id = _transforms->get_component(_entities[idx]);
		
		if (!_transforms->is_valid(trf_id))
		{
			fprintf(stderr, "Invalid transform id %d\n", trf_id);
			return false;
		}

		if (!_meshes->is_valid(mid))
		{
			fprintf(stderr, "Invalid mesh id %d\n", mid);
			return false;
		}

		const matrix4& model = _transforms->get_model(trf_id);
		const matrix4& inv = _transforms->get_model_inv(trf_id);

		bool is_hit = false;
		float nearest = std::numeric_limits<float>::max();
		vector3 nearest_hit;

		vector3 ra = inv * test.origin;
		vector3 rb = inv * (test.origin + test.direction * 1000.0f);

		ray r = ray(ra, (rb - ra).norm());

		const vector3 d = r.direction;
		const vector3 o = r.origin;

		for (size_t i = 0; i < _meshes->get_index_count(mid);)
		{
			const vector3& a = _meshes->get_indexed_position(mid, i++);
			const vector3& b = _meshes->get_indexed_position(mid, i++);
			const vector3& c = _meshes->get_indexed_position(mid, i++);

			const vector3 e1 = b - a;
			const vector3 e2 = c - a;
			const vector3 n = vector3::cross(e2, e1); // Cross product order changed

			const float det = -vector3::dot(d, n);
			const float invdet = 1.0f / det;

			const vector3 ao = o - a;
			const vector3 dao = vector3::cross(d, ao); // Cross product order changed

			const float u = vector3::dot(e2, dao) * invdet;
			const float v = -vector3::dot(e1, dao) * invdet;
			const float t = vector3::dot(ao, n) * invdet;

			if (det >= 1e-6 && t >= 0.0f && u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f)
			{
				is_hit = true;

				if (t < nearest)
				{
					nearest_hit = o + d * t;
					nearest = t;
					norm = n;
				}
			}
		}

		hit = (model * vector4(nearest_hit, 1.0f)).xyz();
		norm = (model * vector4(norm, 1.0f)).xyz();

		return is_hit;
	}
}
