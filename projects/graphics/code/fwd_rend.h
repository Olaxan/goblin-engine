#pragma once

#include "manager.h"
#include "ifmgr.h"

#include "rend_set.h"
#include "mesh_mgr.h"
#include "mtrl_mgr.h"

#include "lght_mgr.h"
#include "cam_mgr.h"

#include <memory>
#include <chrono>

namespace efiilj
{
	typedef int render_id;

	class forward_renderer : public manager<render_id>
	{
	protected:

		const renderer_settings& settings_;

		struct RenderData
		{
		} _data;

		std::shared_ptr<camera_manager> _cameras;
		std::shared_ptr<transform_manager> _transforms;
		std::shared_ptr<light_manager> _lights;
		std::shared_ptr<mesh_manager> _mesh_instances;
		std::shared_ptr<material_manager> _material_instances;
		std::shared_ptr<mesh_server> _meshes;
		std::shared_ptr<material_server> _materials;
		std::shared_ptr<shader_server> _shaders;

	public:

		forward_renderer(const renderer_settings& set);
		~forward_renderer() = default;

		void extend_defaults(render_id new_id) override;
		void draw_gui() override;
		void draw_gui(render_id idx) override;
		
		void on_register(std::shared_ptr<manager_host> host) override;
	
		void render(render_id idx) const;
		virtual void render_frame() const;

		void begin_frame();
		void end_frame();

		virtual void on_begin_frame() {}
		virtual void on_end_frame() {}

	};
}
