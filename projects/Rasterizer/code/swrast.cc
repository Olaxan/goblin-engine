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

	void rasterizer::normalize(vector4& vec, transform_model& transform)
	{
		vec = (camera_->view() * transform.model() * vec);
		vec /= vec.w();
		vec.x((vec.x() + 1) * (width_ / 2));
		vec.y((vec.y() + 1) * (height_ / 2));
	}

	void rasterizer::put_pixel(const int x, const int y, const unsigned int c)
	{
		if (x > 0 && x < width_ && y > 0 && y < height_)
			buffer_[x + width_ * y] = c;
	}

	void rasterizer::fill_line(const point_data& start, const point_data& end)
	{
		const int y = start.y;

		for (int x = std::min(start.x, end.x); x < std::max(start.x, end.x); x++)
		{
			put_pixel(x, y, 0xFFFFFFFF);
		}
	}

	//146469925
	void rasterizer::draw_tri(rasterizer_node& node, const unsigned index)
	{
		
		vertex& v1 = node.get_by_index(index);
		vertex& v2 = node.get_by_index(index + 1);
		vertex& v3 = node.get_by_index(index + 2);

		const vertex_uniforms vertex_u { camera_->view(), node.transform().model() };
		
		const vertex_data vs1 = node.vertex_shader(v1, vertex_u);
		const vertex_data vs2 = node.vertex_shader(v2, vertex_u);
		const vertex_data vs3 = node.vertex_shader(v3, vertex_u);

		vector4 points[] = { vs1.fragment, vs2.fragment, vs3.fragment };
		
		normalize(points[0], node.transform());
		normalize(points[1], node.transform());
		normalize(points[2], node.transform());

		const auto cmp = [&](const vector4& a, const vector4& b) { return a.x() + static_cast<float>(width_) * a.y() < b.x() + static_cast<float>(width_) * b.y(); };
		std::sort(points, points + 3, cmp);

		line_data l1(points[0], points[2]);
		line_data l2(points[0], points[1]);
		line_data l3(points[1], points[2]);

		if (l1.dy > 0 && l2.dy > 0)
		{
			while (l1.curr_y < l2.y2)
			{
				point_data pt1 = point_on_line(l1);
				point_data pt2 = point_on_line(l2);
				fill_line(pt1, pt2);
			}
		}

		if (l1.dy > 0 && l3.dy > 0)
		{
			while (l1.curr_y < l3.y2)
			{
				point_data pt1 = point_on_line(l1);
				point_data pt2 = point_on_line(l3);
				fill_line(pt1, pt2);
			}
		}
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

	bool rasterizer::cull_backface(const vector4& a, const vector4& b, const vector4& c) const
	{
		const vector4 u = b - a;
		const vector4 v = c - a;
		const vector4 face_normal = vector4::cross(u, v);
		const vector4 camera_direction = camera_->transform().forward();
		
		return vector4::dot(camera_direction, face_normal) > 0;
	}

	void rasterizer::bresenham_line(line_data& line, const unsigned c)
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

	void rasterizer::clear()
	{
		std::fill(buffer_, buffer_ + width_ * height_, color_);
	}

	void rasterizer::render()
	{
		clear();

		for (const auto& node_ptr : nodes_)
		{
			for (unsigned int i = 0; i < node_ptr->index_count(); i += 3)
			{
				draw_tri(*node_ptr, i);
			}
		}
	}
}
