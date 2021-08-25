#pragma once

#include "manager.h"
#include "mgr_host.h"

#include "mesh_srv.h"

namespace efiilj
{

	typedef int mesh_instance_id;

	class mesh_manager : public manager<mesh_instance_id>, public registrable
	{
		private:

			struct MeshInstanceData
			{
				std::vector<mesh_id> id;
			} _data;

		public:

			mesh_manager();
			~mesh_manager();

			mesh_instance_id register_entity(entity_id eid) override;
			bool unregister_entity(mesh_instance_id idx) override;

			void draw_gui() override;
			void draw_gui(mesh_instance_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			mesh_id get_mesh(mesh_instance_id idx);
			void set_mesh(mesh_instance_id idx, mesh_id mat_id);
	};
}
