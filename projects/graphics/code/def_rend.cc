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
		forward_renderer(camera_manager, settings),
		rbo_(0), depth_texture_(0), target_texture_(0), ubo_(0), quad_vao_(0), quad_vbo_(0), 
		geometry_(std::move(geometry)), 
		lighting_(std::move(lighting))
	{
		// Setup gbuffer
		glGenFramebuffers(1, &rbo_);
		glBindFramebuffer(GL_FRAMEBUFFER, rbo_);

		unsigned i = 0;
		textures_.push_back(gen_texture(i++, GL_FLOAT));			// Pos
		textures_.push_back(gen_texture(i++, GL_FLOAT));			// Normal
		textures_.push_back(gen_texture(i++, GL_UNSIGNED_BYTE));	// Albedo
		textures_.push_back(gen_texture(i++, GL_UNSIGNED_BYTE));	// ORM

		target_texture_ = gen_texture(i++, GL_FLOAT);				// Final

		depth_texture_ = gen_texture(
				GL_DEPTH_ATTACHMENT, 
				GL_DEPTH_COMPONENT32F,
				GL_DEPTH_COMPONENT,
				GL_FLOAT
				);

		// Check if framebuffer is ready
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		setup_quad();
		setup_uniforms();
		setup_volumes();

	}

	unsigned deferred_renderer::gen_texture(unsigned attach, unsigned internal, unsigned format, unsigned type)
	{
		printf("Generating texture %u...\n", attach);

		unsigned handle;

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, internal, settings_.width, settings_.height, 0, format, type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, handle, 0);

		return handle;
	}

	unsigned deferred_renderer::gen_texture(unsigned index, unsigned type)
	{
		return gen_texture(GL_COLOR_ATTACHMENT0 + index, GL_RGBA16F, GL_RGBA, type);
	}

	void deferred_renderer::attach_textures(tex_type texture) const
	{
		switch (texture)
		{
			case tex_type::component_draw:
			{
				std::vector<unsigned> attach;

				for (size_t i = 0; i < textures_.size(); i++)
				{
					attach.push_back(GL_COLOR_ATTACHMENT0 + i);
				}

				glDrawBuffers(attach.size(), attach.data());	

				break;
			}

			case tex_type::component_read:
			{
				for (size_t i = 0; i < textures_.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, textures_[i]);
					glUniform1i(i, i);
				}

				break;
			}

			case tex_type::target:
			{
				glDrawBuffer(GL_COLOR_ATTACHMENT0 + textures_.size());
				break;
			}
		}
	}	

	void deferred_renderer::setup_quad()
	{
		// https://learnopengl.com/Advanced-Lighting/Deferred-Shading

		float quad[] = {
			-1.0f,	1.0f, 0.0f, 1.0f,  
			-1.0f, -1.0f, 0.0f, 1.0f, 
			 1.0f,	1.0f, 0.0f, 1.0f, 
			 1.0f, -1.0f, 0.0f, 1.0f
			};	

		glGenVertexArrays(1, &quad_vao_);
		glGenBuffers(1, &quad_vbo_);
		glBindVertexArray(quad_vao_);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	
	void deferred_renderer::setup_uniforms()
	{
		if (geometry_->use())
			geometry_->bind_block(settings_.ubo_camera, 0);
	}

	void deferred_renderer::setup_volumes()
	{
		object_loader pl_loader(settings_.p_v_pointlight.c_str());

		if (pl_loader.is_valid())
			v_pointlight_ = std::move(pl_loader.get_resource());
		else 
			fprintf(stderr, "FATAL: Failed to load point light volume!\n");
	}

	void deferred_renderer::set_light_uniforms(const light_source& light) const
	{
		lighting_->set_uniform("source.type", static_cast<int>(light.type));
		lighting_->set_uniform("source.base.color", light.base.color);	
		lighting_->set_uniform("source.base.ambient_intensity", light.base.ambient_intensity);
		lighting_->set_uniform("source.base.diffuse_intensity", light.base.diffuse_intensity);
		lighting_->set_uniform("source.position", light.transform.get_position());
		lighting_->set_uniform("source.direction", light.transform.forward());
		lighting_->set_uniform("source.falloff.constant", light.falloff.constant);
		lighting_->set_uniform("source.falloff.linear", light.falloff.linear);
		lighting_->set_uniform("source.falloff.exponential", light.falloff.exponential);
	}

	void deferred_renderer::draw_directional(const light_source& light) const
	{
		lighting_->set_uniform("light_mvp", matrix4());

		// Draw screenspace quad
		glBindVertexArray(quad_vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void deferred_renderer::draw_pointlight(const light_source& light, float radius) const
	{
		const matrix4& v = camera_mgr_->get_active_camera()->get_view();
		const matrix4& p = camera_mgr_->get_active_camera()->get_perspective();

		matrix4 mvp = p * v * light.transform.get_model();

		lighting_->set_uniform("light_mvp", mvp);

		// Draw pointlight volume
		v_pointlight_->bind();
		v_pointlight_->draw_elements();
		v_pointlight_->unbind();
	}

	void deferred_renderer::on_begin_frame()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rbo_);
		attach_textures(tex_type::target);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void deferred_renderer::render() const
	{

		if (!geometry_->use())
			return;

		/* ---------- Geometry Pass ---------- */
		
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		attach_textures(tex_type::component_draw);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& node : nodes_)
		{
			node->bind();
			node->draw();
			node->unbind();
		}

		glDepthMask(GL_FALSE);

		/* ---------- Lighting Passes ---------- */
		
		if (!lighting_->use())
			return;

		attach_textures(tex_type::target);	
		attach_textures(tex_type::component_read);

		lighting_->set_uniform(settings_.u_camera, camera_mgr_->get_active_position()) ; 
		lighting_->set_uniform(settings_.u_dt_seconds, get_delta_time());

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		for (size_t i = 0; i < light_sources_.size(); i++)
		{
			light_source& light = *light_sources_[i];

			set_light_uniforms(light);

			switch (light.type)
			{
				case light_type::directional:
				{
					draw_directional(light);	
					break;
				}

				case light_type::pointlight:
				{
					
					vector3 cam_pos = camera_mgr_->get_active_position();
					vector3 cam_dir = cam_pos - light.transform.get_position();

					float radius = light.transform.get_scale().length();

					if (cam_dir.length() < radius)
						draw_directional(light);
					else
						draw_pointlight(light, radius);

					break;
				}

				default:
					fprintf(stderr, "ERROR: Light type not implemented!\n");
			}	
		}

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}	

	void deferred_renderer::on_end_frame()
	{
		// Reset GL state for forward pass

		glBindFramebuffer(GL_FRAMEBUFFER, 0);	
		glBindFramebuffer(GL_READ_FRAMEBUFFER, rbo_);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + textures_.size());

		glBlitFramebuffer(0, 0, settings_.width, settings_.height,
						  0, 0, settings_.width, settings_.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	
	void deferred_renderer::reload_shaders() const
	{
		geometry_->reload();
		lighting_->reload();
	}
}
