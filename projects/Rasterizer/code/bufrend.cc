#include "bufrend.h"
#include "GL/glew.h"
#include "shader_res.h"

namespace efiilj
{

	buffer_renderer::buffer_renderer(std::shared_ptr<rasterizer> rasterizer)
		: rasterizer_(std::move(rasterizer)), width_(rasterizer_->get_width()), height_(rasterizer_->get_height())
	{	
		glGenVertexArrays(1, &vao_);
		texture_ = std::make_shared<texture_resource>(width_, height_, rasterizer_->get_frame_buffer());
		shader_ = std::make_shared<shader_resource>(fs_vertex_shader_, fs_fragment_shader_);

		std::cout << "Init buffer renderer..." << std::endl;
	}

	void buffer_renderer::draw() const
	{
		glBindVertexArray(vao_);
		rasterizer_->render();
		texture_->update(rasterizer_->get_frame_buffer());
		
		shader_->use();
		texture_->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		shader_->drop();
		texture_->unbind();
	}
}
