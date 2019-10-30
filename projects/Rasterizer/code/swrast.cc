#include "swrast.h"
#include "GL/glew.h"
#include "line.h"

#include <utility>
#include <cmath>
#include <set>

namespace efiilj
{
	rasterizer::rasterizer(const unsigned int height, const unsigned int width,
		std::shared_ptr<camera_model> camera, const unsigned int color)
		: height_(height), width_(width), color_(color), camera_(std::move(camera))
	{
		buffer_ = new unsigned int[width * height];
		depth_ = new unsigned short[width * height];

		for (int i = 0; i < width * height; i++)
		{
			buffer_[i] = color;
		}
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
		if (0 <= x && x < width_ && 0 <= y && y < height_)
			buffer_[x + width_ * y] = c;
	}

	void rasterizer::fill_line(const vector2& start, const vector2& end)
	{
		int y = start.y();
		for (int x = std::min(start.x(), end.x()); x < std::max(start.x(), end.x()); x++)
		{
			put_pixel(x, y, 0xFFFFFFFF);
		}
	}

	//146469925
	void rasterizer::draw_tri(rasterizer_node& node, const unsigned index)
	{
		const vertex& v1 = node.get_by_index(index);
		const vertex& v2 = node.get_by_index(index + 1);
		const vertex& v3 = node.get_by_index(index + 2);

		vector4 vm1 = v1.xyzw;
		vector4 vm2 = v2.xyzw;
		vector4 vm3 = v3.xyzw;
		
		normalize(vm1, node.transform());
		normalize(vm2, node.transform());
		normalize(vm3, node.transform());

		auto cmp = [](const vector4& a, const vector4& b) { return a.y() < b.y(); };
		auto sorted = std::set<vector4, decltype(cmp)>(cmp) = {vm1, vm2, vm3};

		const auto it_start = sorted.begin();
		const auto it_middle = ++sorted.begin();
		const auto it_end = --sorted.end();

		auto scanline = static_cast<unsigned>(it_start->y());

		line_data l1(*it_start, *it_end);
		line_data l2(*it_start, *it_middle);
		line_data l3(*it_middle, *it_end);

		while (scanline < l2.y2)
		{
			vector2 pt1 = point_on_line(l1);
			vector2 pt2 = point_on_line(l2);
			fill_line(pt1, pt2);
			scanline++;
		}
		
		while (scanline < l3.y2)
		{
			vector2 pt1 = point_on_line(l1);
			vector2 pt2 = point_on_line(l3);
			fill_line(pt1, pt2);
			scanline++;
		}
	}

	vector2 rasterizer::point_on_line(line_data& line)
	{
		// X is major axis
		if (line.dx > line.dy)
		{
			line.x1 += line.step_x;
			if (line.fraction >= 0)
			{
				line.y1 += line.step_y;
				line.fraction -= line.dx;
			}
			line.fraction += line.dy;
			return vector2(line.x1, line.y1);
		}

		// Y is major axis
		{
			if (line.fraction >= 0)
			{
				line.x1 += line.step_x;
				line.fraction -= line.dy;
			}
			line.y1 += line.step_y;
			line.fraction += line.dx;
			return vector2(line.x1, line.y1);
		}
	}

	void rasterizer::bresenham_line(const line_data& line, const unsigned c)
	{
		int x = line.x1;
		int y = line.y1;

		// put initial pixel
		put_pixel(x, y, c);

		if (line.dx > line.dy) // X is major axis
		{
			int fraction = line.dy - (line.dx >> 1);
			
			while (x != line.x2) 
			{
				x += line.step_x;
				if (fraction >= 0) 
				{
					y += line.step_y;
					fraction -= line.dx;
				}
				fraction += line.dy;
				put_pixel(x, y, c);
			}
		}
		else // Y is major axis
		{
			int fraction = line.dx - (line.dy >> 1);
			
			while (y != line.y2) 
			{
				if (fraction >= 0) 
				{
					x += line.step_x;
					fraction -= line.dy;
				}
				y += line.step_y;
				fraction += line.dx;
				put_pixel(x, y, c);
			}
		}	
	}

	void rasterizer::clear()
	{
		memset(buffer_, 0, width_ * height_ * sizeof(unsigned));
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
