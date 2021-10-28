#pragma once

#include "manager.h"
#include "mgr_host.h"

#include "mesh_mgr.h"
#include "mtrl_mgr.h"
#include "lght_mgr.h"
#include "cam_mgr.h"
#include "phys_data.h"
#include "sim.h"

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
				ComponentData<mesh_id> bbox;
				ComponentData<bool> draw_bounds;
				ComponentData<bool> draw_com;
				ComponentData<bool> draw_penetration;
				ComponentData<bool> draw_impulses;
				ComponentData<bool> draw_minowski;
			} _data;

			struct DebugSphere 
			{
				vector3 location;
				float scale;
				size_t frames;

				DebugSphere(const vector3& pos, float scale, size_t frames)
					: location(pos), scale(scale), frames(frames)
				{ }
			};

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
			std::shared_ptr<simulator> _sim;

			void create_bbox_mesh(debug_id idx);
			void create_line_mesh();
			void update_bbox_mesh(debug_id idx);

			std::vector<DebugSphere> spheres;

			mesh_id sphere;
			mesh_id line;

		public:

			debug_renderer();
			~debug_renderer();

			void on_editor_gui() override;
			void on_editor_gui(debug_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;
			void on_setup() override;

			void on_activate(debug_id idx) override;

			void on_begin_frame() override;
			void on_frame() override;
			void on_end_frame() override;

			void render(debug_id idx);
			void render_all();

			void draw_line(const vector3& a, const vector3& b, const vector4& color) const;
			void draw_line(const vector3& a, const vector3& b) const;

			void draw_sphere(const vector3& pos, float size) const;
			void draw_sphere(const vector3& pos, float size, const vector4& color) const;


			void add_debug_sphere(const vector3& pos, float size, size_t frames)
			{
				spheres.emplace_back(pos, size, frames);
			}

			void set_shader(shader_id id)
			{
				_shader = id;
			}
	};

}
