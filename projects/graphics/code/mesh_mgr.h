#pragma once

#include "manager.h"
#include "mgr_host.h"

#include "mesh_srv.h"

namespace efiilj
{

	typedef int mesh_instance_id;

	class mesh_manager : public manager<mesh_instance_id>
	{
		private:

			struct MeshInstanceData
			{
				std::vector<mesh_id> id;
			} _data;

			std::shared_ptr<mesh_server> _meshes;

		public:

			mesh_manager();
			~mesh_manager();

			void extend_defaults(mesh_instance_id) override;
			void draw_gui() override;
			void draw_gui(mesh_instance_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			mesh_id get_mesh(mesh_instance_id idx);
			void set_mesh(mesh_instance_id idx, mesh_id mat_id);
	};
}
