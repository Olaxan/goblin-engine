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
			std::vector<mesh_instance_id> mesh;
			std::vector<material_instance_id> material;
			std::vector<transform_id> transform;
		} _data;

		std::shared_ptr<camera_manager> _cameras;
		std::shared_ptr<transform_manager> _transforms;
		std::shared_ptr<light_manager> _lights;
		std::shared_ptr<mesh_manager> _mesh_instances;
		std::shared_ptr<material_manager> _material_instances;

		std::shared_ptr<mesh_server> _meshes;
		std::shared_ptr<material_server> _materials;

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

		void set_mesh(render_id idx, mesh_instance_id  mesh)
		{
			_data.mesh[idx] = mesh;
		}

		void set_material(render_id idx, material_instance_id mat)
		{
			_data.material[idx] = mat;
		}

		void set_transform(render_id idx, transform_id transform)
		{
			_data.transform[idx] = transform;
		}
	};
}
