#include "swrast.h"
#include "GL/glew.h"

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
		const auto it_middle = sorted.begin()++;
		const auto it_end = sorted.end();

		auto scanline = static_cast<unsigned>(it_start->y());
		
		while (scanline < it_middle->y())
		{
			vector2 pt1 = point_on_line(*it_start, *it_middle, scanline);
			vector2 pt2 = point_on_line(*it_start, *it_end, scanline);
			fill_line(pt1, pt2);
			scanline++;
		}

		while (scanline < it_end->y())
		{
			vector2 pt1 = point_on_line(*it_middle, *it_end, scanline);
			vector2 pt2 = point_on_line(*it_start, *it_end, scanline);
			fill_line(pt1, pt2);
			scanline++;
		}
		
		bresenham_line(vm1.x(), vm1.y(), vm2.x(), vm2.y());
		bresenham_line(vm1.x(), vm1.y(), vm3.x(), vm3.y());
		bresenham_line(vm2.x(), vm2.y(), vm3.x(), vm3.y());
	}

	void rasterizer::bresenham_line(int x1, int y1, const int x2, const int y2, const unsigned c)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;

		const int step_x = (dx < 0) ? -1 : 1;
		const int step_y = (dy < 0) ? -1 : 1;

		dx *= (2 * step_x);
		dy *= (2 * step_y);

		put_pixel(x1, y1, c);

		if (dx > dy)	// X is major axis
		{
			int fraction = dy - (dx >> 1);
			
			while (x1 != x2) 
			{
				x1 += step_x;
				if (fraction >= 0) 
				{
					y1 += step_y;
					fraction -= dx;
				}
				fraction += dy;
				put_pixel(x1, y1, c);
			}
		}
		else			// Y is major axis
		{
			int fraction = dx - (dy >> 1);
			
			while (y1 != y2) 
			{
				if (fraction >= 0) 
				{
					x1 += step_x;
					fraction -= dy;
				}
				y1 += step_y;
				fraction += dx;
				put_pixel(x1, y1, c);
			}
		}	
	}

	void rasterizer::bresenham_line(const float x1, const float y1, const float x2, const float y2, const unsigned c)
	{
		bresenham_line(
			static_cast<int>(round(x1)),
			static_cast<int>(round(y1)),
			static_cast<int>(round(x2)),
			static_cast<int>(round(y2)),
			c
		);
	}

	vector2 rasterizer::point_on_line(const vector4& start, const vector4& end, unsigned scanline)
	{
		
		int x1 = static_cast<int>(start.x());
		int y1 = static_cast<int>(start.y());
		const int y2 = static_cast<int>(end.y());
		const int x2 = static_cast<int>(end.x());
		
		int dx = x2 - x1;
		int dy = y2 - y1;

		const int step_x = (dx < 0) ? -1 : 1;
		const int step_y = (dy < 0) ? -1 : 1;

		dx *= (2 * step_x);
		dy *= (2 * step_y);

		if (dx > dy)	// X is major axis
		{
			int fraction = dy - (dx >> 1);

			x1 += step_x;
			if (fraction >= 0)
			{
				y1 += step_y;
				fraction -= dx;
			}
			fraction += dy;
			return vector2(x1, y1);
		}
		else			// Y is major axis
		{
			int fraction = dx - (dy >> 1);

			if (fraction >= 0)
			{
				x1 += step_x;
				fraction -= dy;
			}
			y1 += step_y;
			fraction += dx;
			return vector2(x1, y1);
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
