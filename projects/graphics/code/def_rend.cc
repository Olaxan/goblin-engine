#include "def_rend.h"
#include "loader.h"

#include "GL/glew.h"
#include <imgui.h>

#include <cassert>
#include <chrono>
#include <memory>

namespace efiilj
{
	deferred_renderer::deferred_renderer(const renderer_settings& settings) 
		: 
		forward_renderer(settings),
		rbo_(0), depth_texture_(0), target_texture_(0), ubo_(0), quad_vao_(0), quad_vbo_(0)
	{

		printf("Init deferred renderer...\n");
		_name = "Deferred renderer";

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

	}

	void deferred_renderer::draw_gui()
	{
	}

	void deferred_renderer::draw_gui(render_id idx) // NOLINT
	{
		unsigned int s1 = _shaders->get_program_id(_fallback_primary);
		unsigned int s2 = _shaders->get_program_id(_fallback_secondary);

		ImGui::BulletText("FBO: %d, UBO: %d", rbo_, ubo_);
		ImGui::BulletText("Default primary: %d / %u", _fallback_primary, s1);
		ImGui::BulletText("Default secondary: %d / %u", _fallback_secondary, s2);
		ImGui::BulletText("Nodes: %lu", _instances.size());
		ImGui::BulletText("Width: %u, Height: %u", settings_.width, settings_.height);

		ImGui::Separator();

		bool err = _data.error[idx];
		bool vis = _data.visible[idx];

		ImGui::TextColored(err ? ImVec4(1, 0, 0, 1) : ImVec4(0, 1, 0, 1), err ? "Model error state!" : "No error detected!");
		ImGui::Text("Visible: %s", vis ? "true" : "false");

	}

	void deferred_renderer::on_register(std::shared_ptr<manager_host> host)
	{
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
		_lights = host->get_manager_from_fcc<light_manager>('LGHT');
		_cameras = host->get_manager_from_fcc<camera_manager>('CAMS');
		_mesh_instances = host->get_manager_from_fcc<mesh_manager>('MEMR');
		_material_instances = host->get_manager_from_fcc<material_manager>('MAMR');

		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');
		_materials = host->get_manager_from_fcc<material_server>('MASR');
		_shaders = host->get_manager_from_fcc<shader_server>('SHDR');

		_fallback_primary = _shaders->create();
		_fallback_secondary = _shaders->create();

		_shaders->set_uri(_fallback_primary, settings_.default_fallback_path_primary);
		_shaders->set_uri(_fallback_secondary, settings_.default_fallback_path_secondary);

		_shaders->compile(_fallback_primary);
		_shaders->compile(_fallback_secondary);

		setup_quad();
		setup_uniforms();
		setup_volumes();
	}

	unsigned deferred_renderer::gen_texture(unsigned attach, unsigned internal, unsigned format, unsigned type)
	{
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

			default:
				fprintf(stderr, "Invalid texture mode!\n");
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
		if (_shaders->use(_fallback_primary) && _shaders->bind_block(_fallback_primary, settings_.ubo_camera, 0))
			printf("Uniform block at location 0\n");
		else
			fprintf(stderr, "Failed to bind primary deferred uniform block!\n");
	}

	void deferred_renderer::setup_volumes()
	{
		object_loader pl_loader(settings_.pointlight_volume_path, _meshes);

		if (pl_loader.is_valid())
			v_pointlight_ = pl_loader.get_mesh();
		else 
			fprintf(stderr, "FATAL: Failed to load point light volume!\n");
	}	

	void deferred_renderer::set_light_uniforms(light_id idx) const
	{
		const light_type& type = _lights->get_type(idx);
		const light_base& base = _lights->get_base(idx);
		const attenuation_data& att = _lights->get_attenuation(idx);
		const cutoff_data& cutoff = _lights->get_cutoff(idx);
		const transform_id& trf = _lights->get_transform(idx);

		_shaders->set_uniform("source.type", static_cast<int>(type));
		_shaders->set_uniform("source.base.color", base.color);
		_shaders->set_uniform("source.base.ambient_intensity", base.ambient_intensity);
		_shaders->set_uniform("source.base.diffuse_intensity", base.diffuse_intensity);
		_shaders->set_uniform("source.position", _transforms->get_position(trf));
		_shaders->set_uniform("source.direction", _transforms->get_forward(trf));
		_shaders->set_uniform("source.falloff.constant", att.constant);
		_shaders->set_uniform("source.falloff.linear", att.linear);
		_shaders->set_uniform("source.falloff.exponential", att.exponential);
		_shaders->set_uniform("source.cutoff.inner", cutoff.inner_angle);
		_shaders->set_uniform("source.cutoff.outer", cutoff.outer_angle);
	}

	void deferred_renderer::draw_directional() const
	{
		_shaders->set_uniform("light_mvp", matrix4());

		// Draw screenspace quad
		glBindVertexArray(quad_vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void deferred_renderer::draw_pointlight(const matrix4& model) const
	{

		camera_id cid = _cameras->get_camera();

		const matrix4& v = _cameras->get_view(cid);
		const matrix4& p = _cameras->get_perspective(cid);

		matrix4 mvp = p * v * model;

		_shaders->set_uniform("light_mvp", mvp);

		_meshes->bind(v_pointlight_);
		_meshes->draw_elements(v_pointlight_);
		_meshes->unbind();
	}

	void deferred_renderer::on_begin_frame()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rbo_);
		attach_textures(tex_type::target);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void deferred_renderer::render_frame()
	{

		camera_id cam = _cameras->get_camera();
		const vector3& cam_pos = _transforms->get_position(_cameras->get_transform(cam));

		/* ---------- Geometry Pass ---------- */
		
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		attach_textures(tex_type::component_draw);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& idx : _instances)
		{
			render(idx);
		}

		glDepthMask(GL_FALSE);

		/* ---------- Lighting Passes ---------- */
		
		if (_shaders->use(_fallback_secondary))
		{

			attach_textures(tex_type::target);	
			attach_textures(tex_type::component_read);

			_shaders->set_uniform(_fallback_secondary, settings_.u_camera, cam_pos); 

			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);
			glEnable(GL_CULL_FACE);

			for (auto& idx : _lights->get_instances())
			{
				set_light_uniforms(idx);

				switch(_lights->get_type(idx))
				{
					case light_type::directional:
					{
						draw_directional();	
						break;
					}

					case light_type::spotlight:
					case light_type::pointlight:
					{
						
						transform_id trf = _lights->get_transform(idx);
						const matrix4& model = _transforms->get_model(trf);

						vector3 cam_dir = cam_pos - _transforms->get_position(trf);

						float radius = _transforms->get_scale(trf).length();

						if (cam_dir.length() < radius)
							draw_directional();
						else
							draw_pointlight(model);

						break;
					}

					default:
						fprintf(stderr, "ERROR: Light type not implemented!\n");
				}	
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
}
