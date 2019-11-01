#include "swrast.h"
#include "GL/glew.h"
#include "line.h"
#include "color.h"

#include <algorithm>
#include <utility>
#include <cmath>

namespace efiilj
{
	rasterizer::rasterizer(const unsigned int height, const unsigned int width,
		std::shared_ptr<camera_model> camera, const unsigned int color)
		: height_(height), width_(width), color_(color), camera_(std::move(camera))
	{
		buffer_ = new unsigned int[width * height];
		depth_ = new unsigned short[width * height];

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
		vertex.pos /= vertex.pos.w();
		vertex.pos.x((vertex.pos.x() + 1) * (width_ / 2));
		vertex.pos.y((vertex.pos.y() + 1) * (height_ / 2));
	}

	void rasterizer::put_pixel(const int x, const int y, const unsigned int c) const
	{
		if (x > 0 && x < width_ && y > 0 && y < height_)
			buffer_[x + width_ * y] = c;
	}

	void rasterizer::fill_line(const point_data& start, const point_data& end, vertex_data* data) const
	{
		const int y = start.y;

		for (int x = std::min(start.x, end.x); x < std::max(start.x, end.x); x++)
		{
			//color c = active_node_->fragment_shader();
			
			put_pixel(x, y, 0xFFFFFFFF);
		}
	}

	void rasterizer::draw_tri(const unsigned index)
	{

		// Get model face vertices
		vertex* vertices[] = 
		{
			active_node_->get_by_index(index),
			active_node_->get_by_index(index + 1),
			active_node_->get_by_index(index + 2)
		};

		const vector4 normal = face_normal(vertices[0]->xyzw, vertices[1]->xyzw, vertices[2]->xyzw);
		
		// Exit early if face normal faces away from camera
		if (cull_backface(normal))
			return;

		// Create uniforms struct using camera view and node model transform
		const vertex_uniforms vertex_u { camera_->view(), active_node_->transform().model() };

		// Get vertex data from node vertex shader
		vertex_data data[] = 
		{
			active_node_->vertex_shader(vertices[0], vertex_u),
			active_node_->vertex_shader(vertices[1], vertex_u),
			active_node_->vertex_shader(vertices[2], vertex_u)
		};

		// Convert vertex data to screenspace coordinates (?)
		convert_screenspace(data[0]);
		convert_screenspace(data[1]);
		convert_screenspace(data[2]);

		std::cout << get_barycentric(data[0].pos, normal, data).to_mem_string() << std::endl;

		// Sort vertex data array based on vertex position
		const auto cmp = [this](const vertex_data& a, const vertex_data& b) { return a.pos.x() + static_cast<float>(width_) * a.pos.y() < b.pos.x() + static_cast<float>(width_) * b.pos.y(); };
		std::sort(data, data + 3, cmp);

		// Create line data based on sorted vertex data
		line_data l1(data[0].pos, data[2].pos);
		line_data l2(data[0].pos, data[1].pos);
		line_data l3(data[1].pos, data[2].pos);

		// Draw "upper" face segment (until middle vertex is reached)
		if (l1.dy > 0 && l2.dy > 0)
		{
			while (l1.curr_y < l2.y2)
			{
				point_data pt1 = point_on_line(l1);
				point_data pt2 = point_on_line(l2);
				fill_line(pt1, pt2, data);
			}
		}

		// Draw rest of face
		if (l1.dy > 0 && l3.dy > 0)
		{
			while (l1.curr_y < l3.y2)
			{
				point_data pt1 = point_on_line(l1);
				point_data pt2 = point_on_line(l3);
				fill_line(pt1, pt2, data);
			}
		}
	}

	vector4 rasterizer::get_barycentric(const vector4& point, const vector4& face_normal, vertex_data* data)
	{
		const float area = face_normal.length();

		const float first = (data[1].pos.y() - data[2].pos.y())* (point.x() - data[2].pos.x()) + (data[2].pos.x() - data[1].pos.x()) * (point.y() - data[2].pos.y());
		const float lower = (data[1].pos.y() - data[2].pos.y()) * (data[0].pos.x() - data[2].pos.x()) + (data[2].pos.x() - data[1].pos.x()) * (data[0].pos.y() - data[2].pos.y());
		const float p1 = first / lower;

		const float second = (data[2].pos.y() - data[0].pos.y()) * (point.x() - data[2].pos.x()) + (data[0].pos.x() - data[2].pos.x()) * (point.y() - data[2].pos.y());
		const float p2 = second / lower;

		const float p3 = 1 - (p1 + p2);

		return vector4(p1, p2, p3, 1);
	}

	vector4 rasterizer::face_normal(const vector4& a, const vector4& b, const vector4& c)
	{
		const vector4 u = b - a;
		const vector4 v = c - a;
		return vector4::cross(u, v);
	}

	point_data rasterizer::point_on_line(line_data& line)
	{
		point_data point { line.curr_x, line.curr_y };
		
		if (line.horizontal)
		{
			while (true) //line.curr_x != line.x2
			{
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

	bool rasterizer::cull_backface(const vector4& face_normal) const
	{
		const vector4 camera_direction = camera_->transform().forward();
		return vector4::dot(camera_direction, face_normal) > 0;
	}

	void rasterizer::bresenham_line(line_data& line, const unsigned c) const
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

	void rasterizer::clear() const
	{
		std::fill(buffer_, buffer_ + width_ * height_, color_);
	}

	void rasterizer::render()
	{
		clear();

		for (const auto& node_ptr : nodes_)
		{
			active_node_ = node_ptr.get();
			
			for (unsigned int i = 0; i < node_ptr->index_count(); i += 3)
			{
				draw_tri(i);
			}
		}

		active_node_ = nullptr;
	}
}
