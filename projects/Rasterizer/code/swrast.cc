#include "swrast.h"
#include "GL/glew.h"

#include <utility>
#include <cmath>
#include <set>

namespace efiilj
{
	rasterizer::rasterizer(const unsigned int height, const unsigned int width,
		std::shared_ptr<camera_model> camera, std::shared_ptr<transform_model> model)
		: height_(height), width_(width), camera_(std::move(camera)), transform_(std::move(model))
	{
		buffer_ = new unsigned int[width * height];
		depth_ = new unsigned short[width * height];
	}

	rasterizer::~rasterizer()
	{
		delete[] buffer_;
		delete[] depth_;
	}

	void rasterizer::put_pixel(const int x, const int y, const unsigned int c) const
	{
		buffer_[x + width_ * y] = c;
	}

	void rasterizer::draw_tri(const vertex& v1, const vertex& v2, const vertex& v3) const
	{
		std::set<vertex> sort = { v1, v2, v3 };
		bresenham_line(v1, v2);
		bresenham_line(v1, v3);
		bresenham_line(v2, v3);
	}

	void rasterizer::bresenham_line(int x1, int y1, int x2, int y2) const
	{
		int dx = x2 - x1;
		int dy = y2 - y1;

		const int step_x = (dx < 0) ? -1 : 1;
		const int step_y = (dy < 0) ? -1 : 1;

		dx *= (2 * step_x);
		dy *= (2 * step_y);

		if (0 <= x1 && x1 < get_width() && 0 <= y1 && y1 < get_height())
			put_pixel(x1, y1, color(255, 0,0));

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
					put_pixel(x1, y1, color(255, 0, 0));
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
					put_pixel(x1, y1, color(255, 0, 0));
			}
		}	
	}

	void rasterizer::bresenham_line(const vertex& v1, const vertex& v2) const
	{
		const vector4 start = get_mvp() * v1.xyzw;
		const vector4 end = get_mvp() * v2.xyzw;

		bresenham_line(
			static_cast<int>(round(start.x())),
			static_cast<int>(round(start.y())),
			static_cast<int>(round(end.x())), 
			static_cast<int>(round(end.y()))
		);
	}


	void rasterizer::set_shader(std::function<void(int)> vertex, std::function<void(int)> fragment)
	{
		vertex_shader_ = std::move(vertex);
		fragment_shader_ = std::move(fragment);
	}

	void rasterizer::set_buffers(std::vector<vertex> vertices, std::vector<unsigned> indices)
	{
		vertices_ = std::move(vertices);
		indices_ = std::move(indices);
	}

	void rasterizer::render()
	{
		for (unsigned int i = 0; i < indices_.size() / 3; i += 3)
		{
			draw_tri(
				vertices_[indices_[i]],
				vertices_[indices_[i + 1]],
				vertices_[indices_[i + 2]]
			);
		}
	}

}
