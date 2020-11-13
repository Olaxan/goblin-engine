#include "swrast.h"
#include "line.h"

#include <algorithm>
#include <utility>

namespace efiilj
{
	rasterizer::rasterizer(const int height, const int width,
	                       std::shared_ptr<camera_model> camera, const unsigned int color)
		: height_(height), width_(width), color_(color), camera_(std::move(camera))
	{
		buffer_ = new unsigned int[width * height];
		depth_ = new float[width * height];
		x_offset_ = static_cast<float>(width_) / 2;
		y_offset_ = static_cast<float>(height_) / 2;

		clear();
	}

	rasterizer::~rasterizer()
	{
		std::cout << "Deleting rasterizer..." << std::endl;

		delete[] buffer_;
		delete[] depth_;
	}

	void rasterizer::convert_screenspace(vertex_data& vertex) const
	{
		vertex.pos /= vertex.pos.w;

		const float x = vertex.pos.x + 1;
		const float y = vertex.pos.y + 1;
		
		vertex.pos.x = x * x_offset_;
		vertex.pos.y = y * y_offset_;
	}

	bool rasterizer::cull_backface(const vector4& p0, const vector4& face_normal, const vector4& camera_local) const
	{
		const vector4 cam_to_tri = p0 - camera_local;

		return vector4::dot(cam_to_tri, face_normal) >= 0;
	}

	bool rasterizer::depth_test(const int x, const int y, const float z) const
	{
		float* mem = &depth_[x + width_ * y];

		if (z < *mem)
		{
			*mem = z;
			return true;
		}
		return false;
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void rasterizer::put_pixel(const int x, const int y, const unsigned int c)
	{
		if (x > 0 && x < width_ && y > 0 && y < height_)
			buffer_[x + width_ * y] = c;
	}

	void rasterizer::fill_scanline(const point_data& start, const point_data& end, const vector4& face_normal, const rasterizer_node& node,
	                           vertex_data* data)
	{
		// Calculate which point is the furthest in each X direction
		// Draw line from left to right
		// Cut pixels outside screen bounds
		const int y = start.y;
		const int x1 = std::max(std::min(start.x, end.x) - 1, 0);
		const int x2 = std::min(std::max(start.x, end.x) + 1, width_ - 1);

		// Return early if outside the raster
		if (y < 0 || y > height_)
			return;

		for (int x = x1; x < x2; x++)
		{
			// Calculate the barycentric weights of the pixel
			vector3 bc = get_barycentric(static_cast<float>(x), static_cast<float>(y), face_normal, data);

			// Exit early if any barycentric weight is negative (as the pixel will be outside the face)
			if (bc.x < 0 || bc.y < 0 || bc.z < 0)
				continue;

			// Interpolate the fragment data using barycentric coordinates
			vertex_data fragment = interpolate_fragment(bc, data);

			// Exit early if the pixel fails depth testing
			if (!depth_test(x, y, fragment.pos.z))
				continue;

			// TODO: Remove hard-coded fragment uniforms
			fragment_uniforms uniform
			{
				fragment.normal,
				fragment.fragment,
				vector4(camera_->get_transform()->get_position(), 1.0f),
				vector4(0.5f, 0.5f, 0.5f, 1),
				vector4(1, 1, 1, 1),
				vector4(2, 2, 2, 1),
				vector4(0.025f, 0, 0.025f, 1),
				1.0f,
				0.5f,
				8
			};

			// Run fragment shader for pixel and put the resulting color in the raster
			const unsigned c = node.fragment_shader(fragment, node.texture(), uniform);

			put_pixel(x, y, c);
		}
	}

	void rasterizer::draw_tri(rasterizer_node& node, const matrix4& local, const unsigned index)
	{
		// Get model face vertices
		vertex* vertices[] =
		{
			node.get_by_index(index),
			node.get_by_index(index + 1),
			node.get_by_index(index + 2)
		};

		const vector4 face_normal = get_face_normal(vertices[0]->xyzw, vertices[1]->xyzw, vertices[2]->xyzw);
		const vector4 camera_local = local * vector4(camera_->get_transform()->get_position(), 1.0f);

		// Exit early if normal is facing away from camera
		if (cull_backface(vertices[0]->xyzw, face_normal, camera_local))
			return;

		// Create uniforms struct using camera view/perspective and node model transform
		// TODO: Send perspective as well, this broke when updating other code.
		const vertex_uniforms vertex_u(camera_->get_view(), node.transform().get_model());

		// Get vertex data from node vertex shader
		vertex_data data[] =
		{
			node.vertex_shader(vertices[0], vertex_u),
			node.vertex_shader(vertices[1], vertex_u),
			node.vertex_shader(vertices[2], vertex_u)
		};

		// Convert vertex data to screen-space coordinates (?)
		convert_screenspace(data[0]);
		convert_screenspace(data[1]);
		convert_screenspace(data[2]);

		// Sort vertex data array based on vertex position
		std::sort(data, data + 3, vertex_comparator_);

		// Create line data based on sorted vertex data
		line_data l1(data[0].pos, data[2].pos);
		line_data l2(data[0].pos, data[1].pos);
		line_data l3(data[1].pos, data[2].pos);

		// Draw "upper" face segment (until middle vertex is reached)
		if (l1.dy > 0 && l2.dy > 0)
		{
			while (l1.curr_y < l2.y2)
			{
				point_data pt1 = get_point_on_line(l1);
				point_data pt2 = get_point_on_line(l2);
				fill_scanline(pt1, pt2, face_normal, node, data);
			}
		}

		// Draw rest of face
		if (l1.dy > 0 && l3.dy > 0)
		{
			while (l1.curr_y < l3.y2)
			{
				point_data pt1 = get_point_on_line(l1);
				point_data pt2 = get_point_on_line(l3);
				fill_scanline(pt1, pt2, face_normal, node, data);
			}
		}
	}

	void rasterizer::draw_line(line_data& line, const unsigned c)
	{
		// put initial pixel
		put_pixel(line.curr_x, line.curr_y, c);

		if (line.horizontal)
		{
			while (line.curr_x != line.x2)
			{
				line.curr_x += line.step_x;
				if (line.fraction >= 0)
				{
					line.curr_y += line.step_y;
					line.fraction -= line.dx;
				}
				line.fraction += line.dy;
				put_pixel(line.curr_x, line.curr_y, c);
			}
		}
		else
		{
			while (line.curr_y != line.y2)
			{
				if (line.fraction >= 0)
				{
					line.curr_x += line.step_x;
					line.fraction -= line.dy;
				}
				line.curr_y += line.step_y;
				line.fraction += line.dx;
				put_pixel(line.curr_x, line.curr_y, c);
			}
		}

		line.reset();
	}

	vector3 rasterizer::get_barycentric(const float x, const float y, const vector4& face_normal, vertex_data* data)
	{
		const float area = face_normal.length();

		const float first = (data[1].pos.y - data[2].pos.y) * (x - data[2].pos.x) + (data[2].pos.x - data[1].pos.x) * (y - data[2].pos.y);
		const float lower = (data[1].pos.y - data[2].pos.y) * (data[0].pos.x - data[2].pos.x) + (data[2].pos.x - data[1].pos.x) * (data[0].pos.y - data[2].pos.y);
		const float p1 = first / lower;

		const float second = (data[2].pos.y - data[0].pos.y) * (x - data[2].pos.x) + (data[0].pos.x - data[2].pos.x) * (y - data[2].pos.y);
		const float p2 = second / lower;

		const float p3 = 1 - (p1 + p2);

		return vector3(p1, p2, p3);
	}

	vector3 rasterizer::get_barycentric(const vector4& point, const vector4& face_normal, vertex_data* data)
	{
		return get_barycentric(point.x, point.y, face_normal, data);
	}

	vector3 rasterizer::get_barycentric(const point_data& point, const vector4& face_normal, vertex_data* data)
	{
		return get_barycentric(static_cast<float>(point.x), static_cast<float>(point.y), face_normal, data);
	}

	vertex_data rasterizer::interpolate_fragment(const vector3& barycentric, vertex_data* data)
	{
		const vector4 position = data[0].pos * barycentric.x + data[1].pos * barycentric.y + data[2].pos * barycentric.z;
		const vector4 fragment = data[0].fragment * barycentric.x + data[1].fragment * barycentric.y + data[2].fragment * barycentric.z;
		const vector4 normal = data[0].normal * barycentric.x + data[1].normal * barycentric.y + data[2].normal * barycentric.z;
		const vector4 color = data[0].color * barycentric.x + data[1].color * barycentric.y + data[2].color * barycentric.z;
		const vector2 uv = data[0].uv * barycentric.x + data[1].uv * barycentric.y + data[2].uv * barycentric.z;

		return vertex_data{position, fragment, normal, color, uv};
	}

	vector4 rasterizer::get_face_normal(const vector4& a, const vector4& b, const vector4& c)
	{
		const vector4 u = b - a;
		const vector4 v = c - a;
		return vector4::cross(u, v);
	}

	float rasterizer::get_winding_order(const vector4& a, const vector4& b, const vector4& c)
	{
		const matrix2 test(
			b.x - a.x,
			c.x - a.x,
			b.y - a.y,
			c.y - a.y
		);

		return test.determinant();
	}

	point_data rasterizer::get_point_on_line(line_data& line)
	{
		point_data point{line.curr_x, line.curr_y};

		// Line is major along the X-axis
		if (line.horizontal)
		{
			// Step along until the error becomes > 0
			while (true)
			{
				// If exceeding error margin, step in the appropriate direction and exit.
				if (line.fraction >= 0)
				{
					line.curr_y += line.step_y;
					line.fraction -= line.dx;
					break;
				}
				line.curr_x += line.step_x;
				line.fraction += line.dy;
				point.x = line.curr_x;
			}
		}
		else
		{
			line.curr_y += line.step_y;
			point.y = line.curr_y;

			if (line.fraction >= 0)
			{
				line.curr_x += line.step_x;
				line.fraction -= line.dy;
			}

			line.fraction += line.dx;
		}

		return point;
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void rasterizer::clear()
	{
		std::fill(buffer_, buffer_ + width_ * height_, color_);
		std::fill(depth_, depth_ + width_ * height_, 1);
	}

	void rasterizer::render()
	{
		clear();

		for (const auto& node_ptr : nodes_)
		{
			matrix4 local = node_ptr->transform().get_model_inv();

			for (unsigned int i = 0; i < node_ptr->index_count(); i += 3)
			{
				draw_tri(*node_ptr, local, i);
			}
		}
	}
}
