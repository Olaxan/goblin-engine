#pragma once

#include "program.h"
#include "camera.h"

#include <vector>

namespace efiilj
{
	struct renderer_settings
	{
		unsigned width = 1024;
		unsigned height = 1024;

		// Default camera
		float fov = 1.30899694f;
		float near = 0.1f;
		float far = 100.0f;
	};

	class deferred_renderer
	{
	private:
		int width, height;
		unsigned gbo, rbo, ubo, pos, norm, cspec, active_camera;
		unsigned attachments[3];

		renderer_settings settings_;

		std::shared_ptr<shader_program> geometry_;
		std::shared_ptr<shader_program> lighting_;

		std::vector<std::shared_ptr<camera_model>> cameras_;

		void gen_buffer(unsigned* handle, unsigned type);
		void setup_ubo();
		void setup_camera();
	public:
		deferred_renderer(renderer_settings& settings, std::shared_ptr<shader_program> geometry, std::shared_ptr<shader_program> lighting);
		~deferred_renderer() = default;

		void render() const;
		void reload_shaders();

		bool set_camera(unsigned active);
		std::shared_ptr<camera_model> get_active_camera() { return cameras_[active_camera]; }
	};
}
