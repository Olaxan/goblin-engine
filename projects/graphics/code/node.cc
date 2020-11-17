#include "node.h"
#include <memory>
#include <utility>

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

		return point.x > b.min.x - e
			&& point.x < b.max.x + e
			&& point.y > b.min.y - e
			&& point.y < b.max.y + e
			&& point.z > b.min.z - e
			&& point.z < b.max.z + e;
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

	bool graphics_node::ray_intersect_triangle(const ray& test, vector3& hit) const
	{
		if (!ray_intersect_bounds(test, hit))
			return false;

		return true;
	}
}
