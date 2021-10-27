#include "mtrl_mgr.h"
#include "imgui.h"

namespace efiilj
{

	material_manager::material_manager()
	{
		printf("Init material instances...\n");
		_name = "Material";
	}

	material_manager::~material_manager()
	{
		printf("Material instance manager exit\n");
	}

	void material_manager::extend_defaults(material_instance_id new_id)
	{
		_data.id.emplace_back(-1);
	}

	void material_manager::on_editor_gui() {}

	void material_manager::on_editor_gui(material_instance_id idx)
	{
		
		material_id mid = _data.id[idx];

		if (!_materials->is_valid(mid))
		{
			ImGui::TextColored(ImVec4(1,0,0,1), "Invalid material ID %d!", mid);
			return;
		}

		_materials->on_editor_gui(mid);

	}

	void material_manager::on_register(std::shared_ptr<manager_host> host)
	{
		_materials = host->get_manager_from_fcc<material_server>('MASR');
		_textures = host->get_manager_from_fcc<texture_server>('TXSR');
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
