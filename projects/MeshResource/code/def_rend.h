#pragma once

#include "program.h"
#include "cam_mgr.h"
#include "node.h"

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

	struct light_data
	{
		vector3 position = vector3(100.0f, 100.0f, 100.0f);
		vector3 color = vector3(1.0f, 1.0f, 1.0f);
		float intensity = 1.0f;
	};

	struct renderer_settings
	{
		unsigned width = 1024;
		unsigned height = 1024;

		// Uniform names
		std::string ubo_camera = "Matrices";

		std::string u_camera = "cam_pos";
		std::string u_dt_seconds = "dt";
		std::string u_sun_color = "sun.color";
		std::string u_sun_pos = "sun.position";
		std::string u_sun_int = "sun.intensity";

		// Light
		light_data sun;
		vector3 ambient_color = vector3(1.0f, 1.0f, 1.0f);
		float ambient_strength = 0.01f;
		float specular_strength = 1.0f;	
		// Default camera
	};

	class deferred_renderer
	{
	private:

		typedef std::chrono::duration<float> duration;
		typedef std::chrono::high_resolution_clock frame_timer; 
		typedef std::chrono::time_point<frame_timer> frame_timer_point;

		unsigned gbo_, rbo_, ubo_, quad_vao_, quad_vbo_;

		frame_timer_point last_frame_;
		renderer_settings settings_;

		std::shared_ptr<camera_manager> camera_mgr_;
		std::shared_ptr<shader_program> geometry_;
		std::shared_ptr<shader_program> lighting_;

		std::vector<std::shared_ptr<graphics_node>> nodes_;
		std::vector<unsigned> buffers_;

		void gen_buffer(unsigned type);
		void attach_buffers();

		void setup_quad();

	public:
		deferred_renderer
			(
				std::shared_ptr<camera_manager> camera_manager,	
				std::shared_ptr<shader_program> geometry, 
				std::shared_ptr<shader_program> lighting,
				renderer_settings& settings
			);

		~deferred_renderer() = default;

		void add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes);

		void render();
		void reload_shaders();

	};
}
