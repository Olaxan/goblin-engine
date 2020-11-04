#pragma once

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

	class deferred_renderer
	{
	private:

		typedef std::chrono::duration<float> duration;
		typedef std::chrono::high_resolution_clock frame_timer; 
		typedef std::chrono::time_point<frame_timer> frame_timer_point;

		unsigned gbo_, rbo_, ubo_, quad_vao_, quad_vbo_, frame_index_;

		frame_timer_point last_frame_;
		renderer_settings settings_;

		std::shared_ptr<camera_manager> camera_mgr_;
		std::shared_ptr<shader_program> geometry_;
		std::shared_ptr<shader_program> lighting_;

		std::shared_ptr<mesh_resource> v_pointlight_;
		std::shared_ptr<mesh_resource> v_spotlight_;

		std::vector<std::shared_ptr<light_source>> light_sources_;
		std::vector<std::shared_ptr<graphics_node>> nodes_;
		std::vector<unsigned> buffers_;

		void gen_buffer(unsigned type);
		void attach_buffers();

		void setup_quad();
		void setup_uniforms();
		
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

		void add_node(std::shared_ptr<graphics_node> node)
		{
			this->nodes_.push_back(node);
		}

		void add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes)
		{
			for (auto& node : nodes)
				add_node(node);
		}

		void add_light(std::shared_ptr<light_source> light)
		{
			this->light_sources_.push_back(light);
		}

		void add_lights(const std::vector<std::shared_ptr<light_source>>& lights) 
		{
			for (auto& light : lights)
				add_light(light);
		}

		void render();
		void reload_shaders();

		unsigned get_frame_index() const { return frame_index_; }
	};
}
