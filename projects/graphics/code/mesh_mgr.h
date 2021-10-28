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
				ComponentData<mesh_id> mesh;
				ComponentData<material_id> material;
			} _data;

			std::shared_ptr<mesh_server> _meshes;
			std::shared_ptr<material_server> _materials;

		public:

			mesh_manager();
			~mesh_manager();

			void on_editor_gui() override;
			void on_editor_gui(mesh_instance_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			mesh_id get_mesh(mesh_instance_id idx)
			{
				return _data.mesh[idx];
			}

			void set_mesh(mesh_instance_id idx, mesh_id mesh)
			{
				_data.mesh[idx] = mesh;
			}

			material_id get_material(mesh_instance_id idx)
			{
				return _data.material[idx];
			}

			void set_material(mesh_instance_id idx, material_id material)
			{
				_data.material[idx] = material;
			}
	};
}
