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
	}

	void rasterizer::put_pixel(const int x, const int y, const unsigned int c)
	{
		buffer_[x + width_ * y] = c;
	}

	void rasterizer::draw_tri(rasterizer_node& node, const unsigned index)
	{
		vertex& v1 = node.get_by_index(index);
		vertex& v2 = node.get_by_index(index + 1);
		vertex& v3 = node.get_by_index(index + 2);

		vector4 vm1 = v1.xyzw;
		vector4 vm2 = v2.xyzw;
		vector4 vm3 = v3.xyzw;

		normalize(vm1, node.transform());
		normalize(vm2, node.transform());
		normalize(vm3, node.transform());

		vm1.x((vm1.x() + 1) * (width_ / 2));
		vm1.y((vm1.y() + 1) * (height_ / 2));
		
		vm2.x((vm2.x() + 1) * (width_ / 2));
		vm2.y((vm2.y() + 1) * (height_ / 2));
		
		vm3.x((vm3.x() + 1) * (width_ / 2));
		vm3.y((vm3.y() + 1) * (height_ / 2));

		bresenham_line(vm1.x(), vm1.y(), vm2.x(), vm2.y());
		bresenham_line(vm1.x(), vm1.y(), vm3.x(), vm3.y());
		bresenham_line(vm2.x(), vm2.y(), vm3.x(), vm3.y());
	}

	void rasterizer::bresenham_line(int x1, int y1, int x2, int y2, unsigned c)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;

		const int step_x = (dx < 0) ? -1 : 1;
		const int step_y = (dy < 0) ? -1 : 1;

		dx *= (2 * step_x);
		dy *= (2 * step_y);

		if (0 <= x1 && x1 < get_width() && 0 <= y1 && y1 < get_height())
			put_pixel(x1, y1, c);

		if (dx > dy) 
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
				if (0 <= x1 && x1 < get_width() && 0 <= y1 && y1 < get_height())
					put_pixel(x1, y1, c);
			}
		}
		else
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
				if (0 <= x1 && x1 < get_width() && 0 <= y1 && y1 < get_height())
					put_pixel(x1, y1, c);
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
