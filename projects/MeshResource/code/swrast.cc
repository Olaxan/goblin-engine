#include "swrast.h"
#include "GL/glew.h"

#include <utility>
#include <cmath>
#include <set>

namespace efiilj
{
	rasterizer::rasterizer(const int height, const int width, std::shared_ptr<camera_model> camera, std::shared_ptr<transform_model> model)
		: height_(height), width_(width), rbo_(), camera_(std::move(camera)), transform_(std::move(model))
	{
		generate_framebuffer();
	}

	void rasterizer::generate_framebuffer()
	{
		glGenRenderbuffers(1, &rbo_);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
		glRenderbufferStorage(rbo_, GL_DEPTH24_STENCIL8, width_, height_);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void rasterizer::put_pixel(int x, int y)
	{
		//TODO: Implement

		// run fragment shader
		// put on frame buffer
	}

	void rasterizer::draw_tri(const vertex& v1, const vertex& v2, const vertex& v3) const
	{
		bind();
		std::set<vertex> sort = { v1, v2, v3 };
		
	}

	void rasterizer::bresenham_line(int x1, int y1, int x2, int y2)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;

		const int step_x = (dx < 0) ? -1 : 1;
		const int step_y = (dy < 0) ? -1 : 1;

		dx *= (2 * step_x);
		dy *= (2 * step_y);

		if ((0 <= x1) && (x1 < get_width()) && (0 <= y1) && (y1 < get_height()))
			put_pixel(x1, y1);

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
				if ((0 <= x1) && (x1 < get_width()) && (0 <= y1) && (y1 < get_height()))
					put_pixel(x1, y1);
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
				if ((0 <= x1) && (x1 < get_width()) && (0 <= y1) && (y1 < get_height()))
					put_pixel(x1, y1);
			}
		}	
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

	void rasterizer::bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	}
	
	void rasterizer::unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void rasterizer::render()
	{
		for (int i = 0; i < indices_.size() / 3; i += 3)
		{
			draw_tri(vertices_[i], vertices_[i + 1], vertices_[i + 2]);
		}
	}

}
