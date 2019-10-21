#include "swrast.h"
#include "GL/glew.h"

#include <utility>
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
	}

	void rasterizer::draw_tri(const vertex& v1, const vertex& v2, const vertex& v3) const
	{
		bind();
		std::set<vertex> sort = { v1, v2, v3 };

		
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
