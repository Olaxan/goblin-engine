#include "mtrl_mgr.h"

namespace efiilj
{

	material_manager::material_manager()
	{
		printf("Init material instances...\n");
	}

	material_manager::~material_manager()
	{
		printf("Material instance manager exit\n");
	}

	material_instance_id material_manager::register_entity(entity_id eid)
	{
		material_instance_id new_id = _instances.size();
		_instances.emplace_back(new_id);
		_instance_mapping.emplace(eid, new_id);

		_data.id.emplace_back(-1);

		return new_id;
	}

	bool material_manager::unregister_entity(material_instance_id idx)
	{
		return false;
	}

	void material_manager::draw_gui() {}
	void material_manager::draw_gui(material_instance_id idx) {}

	void material_manager::on_register(std::shared_ptr<manager_host> host)
	{

	}

	material_id material_manager::get_material(material_instance_id idx)
	{
		return _data.id[idx];
	}

	void material_manager::set_material(material_instance_id idx, material_id mat_id)
	{
		_data.id[idx] = mat_id;
	}
}
