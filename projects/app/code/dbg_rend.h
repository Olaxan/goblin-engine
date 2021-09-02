#pragma once

#include "manager.h"
#include "mgr_host.h"

#include "mesh_mgr.h"
#include "mtrl_mgr.h"
#include "lght_mgr.h"
#include "cam_mgr.h"
#include "phys_data.h"

#include <memory>
#include <chrono>

namespace efiilj
{
	typedef int debug_id;

	class debug_renderer : public manager<debug_id>
	{
		private:

			struct DebugData
			{
				std::vector<mesh_id> bbox;
				std::vector<bool> draw_bounds;
			} _data;

			shader_id _shader;

			std::shared_ptr<camera_manager> _cameras;
			std::shared_ptr<transform_manager> _transforms;
			std::shared_ptr<light_manager> _lights;
			std::shared_ptr<mesh_manager> _mesh_instances;
			std::shared_ptr<material_manager> _material_instances;
			std::shared_ptr<mesh_server> _meshes;
			std::shared_ptr<material_server> _materials;
			std::shared_ptr<shader_server> _shaders;
			std::shared_ptr<collider_manager> _colliders;

			void create_bbox_mesh(debug_id idx);
			void update_bbox_mesh(debug_id idx);

		public:

			debug_renderer();
			~debug_renderer();

			void extend_defaults(debug_id idx) override;
			void draw_gui() override;
			void draw_gui(debug_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			void render(debug_id idx);

			void begin_frame();
			void render_frame();
			void end_frame();

			void set_shader(shader_id id)
			{
				_shader = id;
			}
	};

}
