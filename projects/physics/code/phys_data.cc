#include "phys_data.h"

#include <algorithm>
#include <iterator>
#include <memory>
#include <limits>

#define GJK_MAX_ITERATIONS 64
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

		// I don't care about efficienty,
		// just want to make sure no weirdness tricks me
		std::stringstream ss1;
		std::stringstream ss2;

		for (auto col : _data.broad_collisions[idx])
			ss1 << col << ", ";

		ImGui::Text("Broad: %s", ss1.str().c_str());

		for (auto col : _data.narrow_collisions[idx])
			ss2 << col << ", ";

		ImGui::Text("Narrow: %s", ss2.str().c_str());
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
		entity_id eid = get_entity(idx);

		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return vector3();

		vector3 pos = _transforms->get_position(trf_id);
		vector3 inv_dir = _transforms->get_model_inv(trf_id) * (dir + pos);
		vector3 furthest_point = vector3();
		float max_dot = -1.0f;

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
	
	bool collider_manager::update_simplex(vector3 simplex[4], int& dim, vector3& dir)
	{
		vector3& a = simplex[0];
		vector3& b = simplex[1];
		vector3& c = simplex[2];
		vector3& d = simplex[3];

		switch (dim)
		{
			case 0:
			{
				b = a;
				dir = -a;
				dim = 1;

				return false;
			}

			case 1:
			{
				dir = cross_aba(b - a, -a);

				c = b;
				b = a;
				dim = 2;

				return false;
			}

			case 2:
			{
				vector3 ao = -a;

				vector3 ab = b - a;
				vector3 ac = c - a;

				vector3 abc = vector3::cross(ab, ac);

				vector3 abp = vector3::cross(ab, abc);

				if (IS_ALIGNED(abp, ao))
				{
					c = b;
					b = a;

					dir = cross_aba(ab, ao);

					return false;
				}

				vector3 acp = vector3::cross(abc, ac);

				if (IS_ALIGNED(acp, ao))
				{
					b = a;
					dir = cross_aba(ac, ao);

					return false;
				}

				if (IS_ALIGNED(abc, ao))
				{
					d = c;
					c = b;
					b = a;

					dir = abc;
				}
				else
				{
					d = b;
					b = a;

					dir = -abc;
				}

				dim = 3;

				return false;
			}
			case 3:
			{
				vector3 ao = -a;
				vector3 ab = b - a;
				vector3 ac = c - a;
				vector3 ad = d - a;

				vector3 abc = vector3::cross(ab, ac);
				vector3 acd = vector3::cross(ac, ad);
				vector3 adb = vector3::cross(ad, ab);

				vector3 temp;

				const int over_abc = 0x1;
				const int over_acd = 0x2;
				const int over_adb = 0x4;

				int test = 
					(IS_ALIGNED(abc, ao) ? over_abc : 0) |
					(IS_ALIGNED(acd, ao) ? over_acd : 0) |
					(IS_ALIGNED(adb, ao) ? over_adb : 0);

				switch (test)
				{
					case 0:
						return true;

					case over_abc:
						goto check_one;

					case over_acd:

						b = c;
						c = d;
						ab = ac;
						ac = ad;
						abc = acd;

						goto check_one;

					case over_adb:
						
						c = b;
						b = d;
						ac = ab;
						ab = ad;

						abc = adb;

						goto check_one;

					case over_abc | over_acd:
						goto check_two;

					case over_acd | over_adb:

						temp = b;
						b = c;
						c = d;
						d = temp;

						temp = ab;
						ab = ac;
						ac = ad;
						ad = temp;

						abc = acd;
						acd = adb;

						goto check_two;

					case over_adb | over_abc:

						temp = c;
						c = b;
						b = d;
						d = temp;

						temp = ac;
						ac = ab;
						ab = ad;
						ad = temp;

						acd = abc;
						abc = adb;

						goto check_two;

					default:
						return true;
				}
check_one:

				if (IS_ALIGNED(vector3::cross(abc, ac), ao))
				{
					b = a;
					dir = cross_aba(ac, ao);
					dim = 2;

					return false;
				}

check_one_cont:

				if (IS_ALIGNED(vector3::cross(ab, abc), ao))
				{
					c = b;
					b = a;
					dir = cross_aba(ab, ao);
					dim = 2;

					return false;
				}

				d = c;
				c = b;
				b = a;
				dir = abc;
				dim = 3;

				return false;

check_two:

				if (IS_ALIGNED(vector3::cross(abc, ac), ao))
				{
					b = c;
					c = d;
					ab = ac;
					ac = ad;
					abc = acd;

					goto check_one;
				}

				goto check_one_cont;
			}
		}

		return true;
	}

#define EPA_MAX_ITERATIONS 32
#define EPA_TOLERANCE 0.0001f

	vector3 collider_manager::epa_expand(vector3 simplex[4], collider_id col1, collider_id col2)
	{
		vector3& a = simplex[0];	
		vector3& b = simplex[1];	
		vector3& c = simplex[2];	
		vector3& d = simplex[3];	

		struct face
		{
			vector3 a;
			vector3 b;
			vector3 c;
			vector3 normal;
		};

		std::vector<face> faces;

		// Add termination simplex to face vector
		faces.emplace_back(a, b, c, vector3::cross(b - a, c - a).norm());
		faces.emplace_back(a, c, d, vector3::cross(c - a, d - a).norm());
		faces.emplace_back(a, d, b, vector3::cross(d - a, b - a).norm());
		faces.emplace_back(b, d, c, vector3::cross(d - b, c - b).norm());

		size_t closest_face = 0;

		for (size_t iterations = 0; iterations < EPA_MAX_ITERATIONS; iterations++)
		{
			closest_face = 0;

			float min_dist = vector3::dot(faces[0].a, faces[0].normal);
			size_t num_faces = faces.size();

			for (size_t i = 1; i < num_faces; i++)
			{
				float dist = vector3::dot(faces[i].a, faces[i].normal);
				if (dist < min_dist)
				{
					min_dist = dist;
					closest_face = i;
				}
			}

			vector3 search_dir = faces[closest_face].normal;
			vector3 p = support(col2, search_dir) - support(col1, -search_dir);

			if (vector3::dot(p, search_dir) - min_dist < EPA_TOLERANCE)
			{
				return faces[closest_face].normal * vector3::dot(p, search_dir);		
			}

			struct edge
			{
				vector3 a;
				vector3 b;
			};

			std::vector<edge> edges;

			size_t num_edges = 0;

			for (size_t i = 0; i < num_faces; i++)
			{
				if (IS_ALIGNED(faces[i].normal, p - faces[i].a))
				{
					for (size_t j = 0; j < 3; j++)
					{
						edge current_edge;

						switch (j)
						{
						case 0:
							current_edge = { faces[i].a, faces[i].b };
							break;
						case 1:
							current_edge = { faces[i].b, faces[i].c };
							break;
						case 2:
							current_edge = { faces[i].c, faces[i].a };
							break;
						}

						bool has_edge = false;
						num_edges = edges.size();

						for (size_t k = 0; k < num_edges; k++)
						{
							if (edges[k].b == current_edge.a && edges[k].a == current_edge.b)
							{
								edges[k].a = edges[num_edges - 1].a;
								edges[k].b = edges[num_edges - 1].b;
								edges.pop_back();
								has_edge = true;
								break;
							}
						}

						if (!has_edge)
							edges.emplace_back(current_edge);

					}

					faces[i] = faces[num_faces - 1];
					faces.pop_back();
					i--;

				}
			}

			for (size_t i = 0; i < num_edges; i++)
			{
				face& last_face = faces[num_faces];
				last_face.a = edges[i].a;
				last_face.b = edges[i].b;
				last_face.c = p;
				last_face.normal = vector3::cross(
						edges[i].a - edges[i].b, 
						edges[i].a - p).norm();

				const float bias = 0.000001f;

				if (IS_NOT_ALIGNED(last_face.a, last_face.normal + bias))
				{
					vector3 temp = last_face.a;
					last_face.a = last_face.b;
					last_face.b = temp;
					last_face.normal = -last_face.normal;
				}
			}
		}
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

		// Arbitrary starting direction
		vector3 search_dir = vector3(0, 0, 1);

		// Simplex is non-initialized
		int dim = 0;

		for (size_t i = 0; i < GJK_MAX_ITERATIONS; i++)
		{
			a = support(col2, search_dir) - support(col1, -search_dir);

			if (IS_NOT_ALIGNED(a, search_dir))
				return false;

			if (update_simplex(simplex, dim, search_dir))
				return true;
		}

		return true;
	}
			
	void collider_manager::update_narrow()
	{
		for (auto idx : _instances)
			_data.narrow_collisions[idx].clear();

		for (auto idx : _instances)
		{
			for (auto col : _data.broad_collisions[idx])
			{
				if (collides_with(idx, col))
					continue;

				if (check_gjk_intersect(idx, col))
				{
					_data.narrow_collisions[idx].insert(col);
					_data.narrow_collisions[col].insert(idx);
				}
			}
		}
	}

	void collider_manager::update()
	{
		for (auto& idx : _instances)
		{
			update_bounds(idx);
		}

		update_broad();
		update_narrow();
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
