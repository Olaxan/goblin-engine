#include "node.h"

#include <memory>
#include <utility>
#include <limits>

namespace efiilj
{

	graphics_node::graphics_node(
		std::shared_ptr<mesh_resource> mesh_ptr,
		std::shared_ptr<material_base> material_ptr,
		std::shared_ptr<transform_model> transform_ptr)
			: mesh_(std::move(mesh_ptr)), material_(std::move(material_ptr)), transform_(std::move(transform_ptr)),
			is_absolute_(false), name("Object")
	{}

	graphics_node::graphics_node(
			std::shared_ptr<mesh_resource> mesh_ptr,
			std::shared_ptr<material_base> material_ptr)
		: graphics_node(mesh_ptr, material_ptr, std::make_shared<transform_model>())
	{}

	void graphics_node::bind() const
	{
		mesh_->bind();

		if (material_ != nullptr)
			material_->apply();
	}

	void graphics_node::unbind() const
	{
		mesh_->unbind();
	}

	void graphics_node::draw() const
	{
		matrix4 model = is_absolute_ ? matrix4() : transform_->get_model();
		material_->get_program()->set_uniform("model", model);
		mesh_->draw_elements();
	}

	bounds graphics_node::get_bounds() const
	{
		return mesh_->get_bounds(transform_->get_model());
	}

	bool graphics_node::point_inside_bounds(const vector3& point) const
	{
		bounds b = get_bounds();

		const float e = 0.0001f;

		return point.x > (b.min.x - e)
			&& point.x < (b.max.x + e)
			&& point.y > (b.min.y - e)
			&& point.y < (b.max.y + e)
			&& point.z > (b.min.z - e)
			&& point.z < (b.max.z + e);
	}

	bool graphics_node::ray_intersect_bounds(const ray& test, vector3& hit) const
	{
		bounds b = get_bounds();

		plane planes[] = {
			plane(b.min.x, vector3(1, 0, 0)),
			plane(b.min.y, vector3(0, 1, 0)),
			plane(b.min.z, vector3(0, 0, 1)),

			plane(b.max.x, vector3(1, 0, 0)),
			plane(b.max.y, vector3(0, 1, 0)),
			plane(b.max.z, vector3(0, 0, 1))
		};

		for (plane& p : planes)
		{
			vector3 result;
			if (test.intersect(p, result) && point_inside_bounds(result))
			{
				hit = result;
				return true;
			}
		}

		return false;
	}

	bool graphics_node::ray_intersect_triangle(const ray& test, vector3& hit, vector3& norm) const
	{

		if (!ray_intersect_bounds(test, hit))
			return false;

		if (!mesh_->has_mesh_data())
			return false;

		bool is_hit = false;
		float nearest = std::numeric_limits<float>::max();
		vector3 nearest_hit;

		ray r = ray((transform_->get_model_inv() * vector4(test.origin, 1.0f)).xyz(),
				(transform_->get_model_inv() * vector4(test.origin + test.direction * 100.0f, 1.0f)).xyz());

		const vector3 d = r.direction;
		const vector3 o = r.origin; 

		auto data = mesh_->get_mesh_data();

		for (size_t i = 0; i < data->indices.size();)
		{
			const vector3& a = data->positions[data->indices[i++]];
			const vector3& b = data->positions[data->indices[i++]];
			const vector3& c = data->positions[data->indices[i++]];

			const vector3 e1 = b - a;
			const vector3 e2 = c - a;
			const vector3 n = vector3::cross(e1, e2);

			const float det = -vector3::dot(d, n);
			const float invdet = 1.0f / det;

			const vector3 ao = o - a;
			const vector3 dao = vector3::cross(ao, d);

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

		hit = (transform_->get_model() * vector4(nearest_hit, 1.0f)).xyz();
		norm = (transform_->get_model() * vector4(norm, 1.0f)).xyz();

		return is_hit;
	}
}
