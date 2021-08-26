#pragma once

#include "fwd_rend.h"
#include "rend_set.h"

#include "lght_mgr.h"
#include "cam_mgr.h"
#include "shdr_mgr.h"

#include <vector>
#include <string>

namespace efiilj
{
	struct buffer_data
	{
		unsigned id;
		unsigned type;
		unsigned slot;
	};	

	class deferred_renderer : public forward_renderer
	{
	private:

		enum class tex_type
		{
			component_draw,
			component_read,
			target
		};

		unsigned gen_texture(unsigned index, unsigned type);
		unsigned gen_texture(unsigned attach, unsigned internal, unsigned format, unsigned type);
		void attach_textures(tex_type textures) const;

		void setup_quad();
		void setup_uniforms();
		void setup_volumes();

		void set_light_uniforms(light_id idx) const;
		void draw_directional() const;
		void draw_pointlight(const matrix4& model) const;

		unsigned rbo_, depth_texture_, target_texture_, ubo_, quad_vao_, quad_vbo_, frame_index_;

		shader_id _primary;
		shader_id _secondary;

		//std::shared_ptr<mesh_resource> v_pointlight_;
		//std::shared_ptr<mesh_resource> v_spotlight_;

		std::vector<unsigned> textures_;

		std::shared_ptr<shader_server> _shaders;

	public:

		deferred_renderer(const renderer_settings& settings);

		~deferred_renderer() = default;

		void draw_gui() override;
		void draw_gui(render_id idx) override;

		void on_register(std::shared_ptr<manager_host> host) override;

		void render_frame() const override;

		void on_begin_frame() override;
		void on_end_frame() override;

	};
}
