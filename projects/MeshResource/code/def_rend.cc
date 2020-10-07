#include "def_rend.h"

#include "GL/glew.h"

#include <cassert>
#include <chrono>

namespace efiilj
{
	deferred_renderer::deferred_renderer
		(
			std::shared_ptr<camera_manager> camera_manager,	
			std::shared_ptr<shader_program> geometry_, 
			std::shared_ptr<shader_program> lighting,
			renderer_settings& settings
		) : 
			gbo_(0), rbo_(0), ubo_(0), quad_vao_(0), quad_vbo_(0),   
			camera_mgr_(std::move(camera_manager)),
			geometry_(std::move(geometry_)), 
			lighting_(std::move(lighting)),
			settings_(settings) 
	{
		// Setup gbuffer
		glGenFramebuffers(1, &gbo_);
		glBindFramebuffer(GL_FRAMEBUFFER, gbo_);

		gen_buffer(GL_FLOAT);		// Pos
		gen_buffer(GL_FLOAT);		// Normal
		gen_buffer(GL_UNSIGNED_BYTE);	// Albedo
		gen_buffer(GL_UNSIGNED_BYTE);	// ORM

		attach_buffers();

		// Setup render buffer + depth
		glGenRenderbuffers(1, &rbo_);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, settings_.width, settings_.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);

		// Check if framebuffer is ready
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		setup_quad();

		//geometry_->bind_block("Matrices", 0); DON'T FORGET

		last_frame_ = frame_timer::now(); 
	}

	void deferred_renderer::gen_buffer(unsigned type)
	{
		unsigned index = buffers_.size();
		
		buffers_.push_back(0);

		unsigned* handle = &buffers_[index];

		glGenTextures(1, handle);
		glBindTexture(GL_TEXTURE_2D, *handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, settings_.width, settings_.height, 0, GL_RGBA, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, *handle, 0);
	}

	void deferred_renderer::attach_buffers()
	{
		std::vector<unsigned> attach;

		for (size_t i = 0; i < buffers_.size(); i++)
		{
			attach.push_back(GL_COLOR_ATTACHMENT0 + i);
		}

		glDrawBuffers(attach.size(), attach.data());
	}	

	void deferred_renderer::setup_quad()
	{
		// https://learnopengl.com/Advanced-Lighting/Deferred-Shading

		float quad[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        	};	

		glGenVertexArrays(1, &quad_vao_);
		glGenBuffers(1, &quad_vbo_);
		glBindVertexArray(quad_vao_);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	void deferred_renderer::add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes)
	{
		for (auto& node : nodes)
			this->nodes_.push_back(node);
	}

	void deferred_renderer::render() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gbo_);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (geometry_->use())
		{
			// Geometry pass	
			
			duration dt = frame_timer::now() - last_frame_;
			last_frame_ = frame_timer::now();	
			lighting_->set_uniform(settings_.u_dt_seconds, dt.count());
			
			for (auto& node : nodes_)
			{
				node->draw();
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (lighting_->use())
		{
			// Lighting pass

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Bind buffer textures
			for (size_t i = 0; i < buffers_.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, buffers_[i]);
				glUniform1i(i, i);
			}

			// Set lighting uniforms (move to UBO)
			lighting_->set_uniform(settings_.u_camera, camera_mgr_->get_active_camera()->get_transform()->position); // nasty
			lighting_->set_uniform("light.color", settings_.sun.color);
			lighting_->set_uniform("light.intensity", settings_.sun.intensity);
			lighting_->set_uniform("light.position", settings_.sun.position);
			lighting_->set_uniform("ambient_color", settings_.ambient_color);
			lighting_->set_uniform("ambient_strength", settings_.ambient_strength);
			lighting_->set_uniform("specular_strength", settings_.specular_strength);

			// Draw screenspace quad
			glBindVertexArray(quad_vao_);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
			
			// Copy depth data
			glBindFramebuffer(GL_READ_FRAMEBUFFER, gbo_);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer
			(
				0, 0, settings_.width, settings_.height, 
				0, 0, settings_.width, settings_.height, 
				GL_DEPTH_BUFFER_BIT, GL_NEAREST
			);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
	
	void deferred_renderer::reload_shaders()
	{
		geometry_->reload();
		lighting_->reload();
	}
}
