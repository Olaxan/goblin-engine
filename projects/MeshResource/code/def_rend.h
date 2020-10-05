#pragma once

#include "program.h"
#include "camera.h"
#include "node.h"

#include <vector>
#include <chrono>
#include <string>

namespace efiilj
{
	struct renderer_settings
	{
		unsigned width = 1024;
		unsigned height = 1024;

		// Uniform names
		std::string uniform_camera = "cam_pos";
		std::string uniform_dt_seconds = "dt";

		// Default camera
		float fov = 1.30899694f;
		float near = 0.1f;
		float far = 100.0f;
	};

	class deferred_renderer
	{
	private:

		typedef std::chrono::duration<float> duration;
		typedef std::chrono::high_resolution_clock frame_timer; 
		typedef std::chrono::time_point<frame_timer> frame_timer_point;

		unsigned gbo_, rbo_, ubo_, pos_, norm_, cspec_, active_camera_;
		unsigned attachments_[3];

		frame_timer_point last_frame_;
		renderer_settings settings_;

		std::shared_ptr<shader_program> geometry_;
		std::shared_ptr<shader_program> lighting_;

		std::vector<std::shared_ptr<graphics_node>> nodes_;
		std::vector<std::shared_ptr<camera_model>> cameras_;	

		void gen_buffer(unsigned* handle, unsigned type);
		void setup_ubo();
		void setup_camera();
	public:
		deferred_renderer(renderer_settings& settings, std::shared_ptr<shader_program> geometry, std::shared_ptr<shader_program> lighting);
		~deferred_renderer() = default;

		void add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes);

		void render();
		void reload_shaders();

		bool set_camera(unsigned active);
		std::shared_ptr<camera_model> get_active_camera() { return cameras_[active_camera_]; }
	};
}
