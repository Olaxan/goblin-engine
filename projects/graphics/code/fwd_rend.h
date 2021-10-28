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

		shader_id _fallback_primary;

		struct RenderData
		{
			ComponentData<bool> visible { true };
			ComponentData<bool> error	{ false };
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

		void on_register(std::shared_ptr<manager_host> host) override;
		void on_setup() override;

		void on_editor_gui() override;
		void on_editor_gui(render_id idx) override;

		void on_begin_frame() override;
		void on_frame() override;
		void on_end_frame() override;

		void render(render_id idx);
		void render_all();

		shader_id get_fallback_shader()
		{ return _fallback_primary; }

		void set_fallback_shader(shader_id sid)
		{ _fallback_primary = sid; }

		bool get_error(render_id idx) const
		{ return _data.error[idx]; }

		void set_error(render_id idx, bool state)
		{ _data.error[idx] = state; }

		bool get_visible(render_id idx)
		{ return _data.visible[idx]; }

		void set_visible(render_id idx, bool state)
		{ _data.visible[idx] = state; }
	};
}
