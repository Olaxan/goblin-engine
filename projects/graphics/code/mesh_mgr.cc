#include "mesh_mgr.h"

namespace efiilj
{

	mesh_manager::mesh_manager()
	{
		printf("Init mesh instances...\n");
	}

	mesh_manager::~mesh_manager()
	{
		printf("Mesh instance manager exit\n");
	}

	mesh_instance_id mesh_manager::register_entity(entity_id eid)
	{
		mesh_instance_id new_id = _instances.size();
		_instances.emplace_back(new_id);
		_instance_mapping.emplace(eid, new_id);

		_data.id.emplace_back(-1);

		return new_id;
	}

	bool mesh_manager::unregister_entity(mesh_instance_id idx)
	{
		return false;
	}

	void mesh_manager::draw_gui() {}
	void mesh_manager::draw_gui(mesh_instance_id idx) {}

	void mesh_manager::on_register(std::shared_ptr<manager_host> host)
	{
		
	}

	mesh_id mesh_manager::get_mesh(mesh_instance_id idx)
	{
		return _data.id[idx];
	}

	void mesh_manager::set_mesh(mesh_instance_id idx, mesh_id mat_id)
	{
		_data.id[idx] = mat_id;
	}
}
