#include "def_rend.h"
#include "loader.h"

#include "GL/glew.h"

#include <cassert>
#include <chrono>
#include <memory>

namespace efiilj
{
	deferred_renderer::deferred_renderer
		(
			std::shared_ptr<camera_manager> camera_manager,
			const renderer_settings& settings,
			std::shared_ptr<shader_program> geometry, 
			std::shared_ptr<shader_program> lighting
		) : 
			gbo_(0), rbo_(0), ubo_(0), quad_vao_(0), quad_vbo_(0),   
			camera_mgr_(std::move(camera_manager)),
			geometry_(std::move(geometry)), 
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

		// Setup render buffer + depth
		glGenTextures(1, &rbo_);
		glBindTexture(GL_TEXTURE_2D, rbo_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, settings_.width, settings_.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rbo_, 0);

		attach_buffers();

		buffers_.push_back(rbo_);

		// Check if framebuffer is ready
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		setup_quad();
		setup_uniforms();

		last_frame_ = frame_timer::now(); 

		object_loader pl_loader(settings_.p_v_pointlight.c_str());
		if (pl_loader.is_valid())
		{
			light_source_ = pl_loader.get_resource();
		}
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
	
	void deferred_renderer::setup_uniforms()
	{
		if (geometry_->use())
			geometry_->bind_block(settings_.ubo_camera, 0);
	}

	void deferred_renderer::set_light_uniforms(const light_source& light)
	{
		lighting_->set_uniform("source.type", static_cast<int>(light.type));
		lighting_->set_uniform("source.base.rgb", light.base.rgb);	
		lighting_->set_uniform("source.base.ambient_intensity", light.base.ambient_intensity);
		lighting_->set_uniform("source.base.diffuse_intensity", light.base.diffuse_intensity);
		lighting_->set_uniform("source.position", light.position);
		lighting_->set_uniform("source.direction", light.direction);
		lighting_->set_uniform("source.falloff.constant", light.falloff.constant);
		lighting_->set_uniform("source.falloff.linear", light.falloff.linear);
		lighting_->set_uniform("source.falloff.exponential", light.falloff.exponential);
	}

	float get_attenuation_radius(const light_source& light)
	{
		float max_channel = fmax(fmax(light.base.rgb.x(), light.base.rgb.y()), light.base.rgb.z());

    	float ret = (-light.falloff.linear + 
					sqrtf(light.falloff.linear * 
							light.falloff.linear - 4 * 
							light.falloff.exponential * (
								light.falloff.exponential - 256 * 
								max_channel * light.base.diffuse_intensity
							)
						)
					) / (2 * light.falloff.exponential);

    	return ret;
	}

	void deferred_renderer::add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes)
	{
		for (auto& node : nodes)
			this->nodes_.push_back(node);
	}

	void deferred_renderer::render() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glBindFramebuffer(GL_FRAMEBUFFER, gbo_);

		if (!geometry_->use())
			return;

		// ---------- Geometry Pass ---------- \\
		
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		
		duration dt = frame_timer::now() - last_frame_;
		last_frame_ = frame_timer::now();	
		lighting_->set_uniform(settings_.u_dt_seconds, dt.count());

		for (auto& node : nodes_)
		{
			node->draw();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ---------- Lighting Passes ----------
		
		if (!lighting_->use())
			return;

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind buffer textures
		for (size_t i = 0; i < buffers_.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, buffers_[i]);
			glUniform1i(i, i);
		}

		lighting_->set_uniform(settings_.u_camera, camera_mgr_->get_active_position()) ; 

		// ---------- Point Lights ---------- 

		for (size_t i = 0; i < light_sources_.size(); i++)
		{
			light_source& light = light_sources_[i];

			set_light_uniforms(light);	

			matrix4 scale = matrix4::get_scale(get_attenuation_radius(light));
			matrix4 pos = matrix4::get_translation(vector4(light.position, 1)); 
			matrix4 model = pos * scale;

			lighting_->set_uniform("light_model", model);
			v_pointlight_.draw_elements();
		}

		// ---------- Directional Light ----------

		set_light_uniforms(directional_light_);

		// Draw screenspace quad
		glBindVertexArray(quad_vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	

		glDisable(GL_BLEND);
}
	
	void deferred_renderer::reload_shaders()
	{
		geometry_->reload();
		lighting_->reload();
	}
}
