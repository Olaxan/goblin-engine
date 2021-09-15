#include "phys_data.h"

#include <algorithm>
#include <iterator>
#include <memory>
#include <limits>

#define GJK_MAX_ITERATIONS 64
#define IS_ALIGNED(a, b) vector3::dot(a, b) < 0

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
		_data.bounds_updated.emplace_back(false);
		_data.broad_collisions.emplace_back();
		_data.narrow_collisions.emplace_back();

		update_bounds(idx);
	}

	void collider_manager::on_register(std::shared_ptr<manager_host> host)
	{
		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');
		_mesh_instances = host->get_manager_from_fcc<mesh_manager>('MEMR');
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
	}

	void collider_manager::draw_gui()
	{}

	void collider_manager::draw_gui(collider_id idx)
	{

		ImGui::Text("Collider %d", idx);

		std::stringstream ss;

		for (auto col : _data.broad_collisions[idx])
			ss << col << ", ";

		ImGui::Text("Broad: %s", ss.str().c_str());
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

			if (!_meshes->is_valid(mid))
				continue;

			min = vector3::min(min, _meshes->get_min(mid));
			max = vector3::max(max, _meshes->get_max(mid));
		}

		_data.mesh_bounds[idx] = bounds(min, max);
		_data.bounds_updated[idx] = true;

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

		std::set<point, point_comp> points_x;
		std::set<point, point_comp> points_y;
		std::set<point, point_comp> points_z;

		for (const auto& idx : _instances)
		{
			const auto& bounds = get_bounds_world(idx);

			points_x.emplace(idx, bounds.min.x);
			points_x.emplace(idx, bounds.max.x);

			points_y.emplace(idx, bounds.min.y);
			points_y.emplace(idx, bounds.max.y);

			points_z.emplace(idx, bounds.min.z);
			points_z.emplace(idx, bounds.max.z);

			_data.broad_collisions[idx].clear();
		}

		std::map<collider_id, std::set<collider_id>> hits_x;
		std::map<collider_id, std::set<collider_id>> hits_y;
		std::map<collider_id, std::set<collider_id>> hits_z;

		check_axis_sweep(points_x, hits_x);
		check_axis_sweep(points_y, hits_y);
		check_axis_sweep(points_z, hits_z);

		for (const auto& idx : _instances)
		{
			std::set<collider_id> ixy;
			std::set<collider_id> ixyz;

			std::set_intersection(hits_x[idx].begin(), hits_x[idx].end(), hits_y[idx].begin(), hits_y[idx].end(),
					std::inserter(ixy, ixy.begin()));

			std::set_intersection(ixy.begin(), ixy.end(), hits_z[idx].begin(), hits_z[idx].end(),
					std::inserter(ixyz, ixyz.begin()));

			_data.broad_collisions[idx] = ixyz;
		}
	}

	vector3 collider_manager::support(collider_id idx, const vector3& dir) const
	{
		return vector3();
	}

	bool collider_manager::check_simplex3(vector3 simplex[4], int dim, vector3& dir)
	{

	}

	bool collider_manager::check_simplex4(vector3 simplex[4], int dim, vector3& dir)
	{

	}
	
	bool collider_manager::check_gjk_intersect(collider_id col1, collider_id col2)
	{
		if (!(is_valid(col1) && is_valid(col2)))
			return false;

		entity_id eid1 = get_entity(col1);
		entity_id eid2 = get_entity(col2);

		transform_id trf1 = _transforms->get_component(eid1);
		transform_id trf2 = _transforms->get_component(eid2);

		if (!(_transforms->is_valid(trf1) && _transforms->is_valid(trf2)))
			return false;

		vector3 simplex[4];
		vector3& a = simplex[0];
		vector3& b = simplex[1];
		vector3& c = simplex[2];
		vector3& d = simplex[3];

		vector3 search_dir = _transforms->get_position(trf1) - _transforms->get_position(trf2);

		c = support(col2, search_dir) - support(col1, -search_dir);
		search_dir = -c;
		b = support(col2, search_dir) - support(col1, -search_dir);

		if (IS_ALIGNED(b, search_dir))
			return false;

		search_dir = vector3::cross(vector3::cross(c - b, -b), c - b);

		/*if (search_dir.is_zero())
		{
			search_dir = vector3::cross(c - b, vector3(1, 0, 0));

			if (search_dir.is_zero())
				search_dir = vector3::cross(c - b, vector3(0, 0, -1));
		}*/

		int dim = 2;

		for (size_t i = 0; i < GJK_MAX_ITERATIONS; i++)
		{
			a = support(col2, search_dir) - support(col1, -search_dir);

			if (IS_ALIGNED(a, search_dir))
				return false;

			dim++;
			if (dim == 3)
				check_simplex3(simplex, dim, search_dir);
			else if (check_simplex4(simplex, dim, search_dir))
				return true;
		}

		return false;
	}

	void collider_manager::update_narrow()
	{
		vector3 s; // = support(random);

		std::vector<vector3> simplex;
		vector3 d = -s;

		while (true)
		{
			vector3 a; // = support(d);

		}
	}

	void collider_manager::update()
	{
		for (auto& idx : _instances)
		{
			update_bounds(idx);
		}

		update_broad();
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
