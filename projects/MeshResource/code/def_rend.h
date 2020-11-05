#pragma once

#include "fwd_rend.h"
#include "program.h"
#include "cam_mgr.h"
#include "rend_set.h"
#include "node.h"
#include "light.h"

#include <vector>
#include <chrono>
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

		unsigned gbo_, rbo_, ubo_, quad_vao_, quad_vbo_, frame_index_;

		std::shared_ptr<shader_program> geometry_;
		std::shared_ptr<shader_program> lighting_;

		std::shared_ptr<mesh_resource> v_pointlight_;
		std::shared_ptr<mesh_resource> v_spotlight_;

		std::vector<std::shared_ptr<light_source>> light_sources_;
		std::vector<unsigned> buffers_;

		void gen_buffer(unsigned type);
		void attach_buffers();

		void setup_quad();
		void setup_uniforms();
		void setup_volumes();

		void set_light_uniforms(const light_source& light) const;
		void draw_directional(const light_source& light) const;
		void draw_pointlight(const light_source& light, float radius) const;

	public:

		deferred_renderer
			(
				std::shared_ptr<camera_manager> camera_manager,	
				const renderer_settings& settings,
				std::shared_ptr<shader_program> geometry, 
				std::shared_ptr<shader_program> lighting
			);

		~deferred_renderer() = default;

		void add_light(std::shared_ptr<light_source> light)
		{
			this->light_sources_.push_back(light);
		}

		void add_lights(const std::vector<std::shared_ptr<light_source>>& lights) 
		{
			for (auto& light : lights)
				add_light(light);
		}

		void reload_shaders() const override;
		void render() const override;

		void on_begin_frame() override;
		void on_end_frame() override;

	};
}
