#include "mesh_mgr.h"
#include "imgui.h"

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

	void mesh_manager::draw_gui()
	{
		static int selected_mesh = -1;

		if (ImGui::TreeNode("Meshes"))
		{
			ImGui::BeginChild("Primitives", ImVec2(0, 100), true);
			for (int i = 0; i < static_cast<int>(_instances.size()); i++)
			{
				char label[128];
				sprintf(label, "Mesh %d", i);
				if (ImGui::Selectable(label, selected_mesh == i))
					selected_mesh = i;
			}
			ImGui::EndChild();

			draw_gui(selected_mesh);

			ImGui::TreePop();
		}
	}

	void mesh_manager::draw_gui(mesh_instance_id idx)
	{
		if (!is_valid(idx))
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "No mesh selected");
			return;
		}

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Mesh %d", idx);

		if (ImGui::TreeNode("Mesh data"))
		{
			ImGui::BulletText("%ld vertices", _meshes->get_vertex_count(idx));
			ImGui::BulletText("%ld indices", _meshes->get_index_count(idx));
			ImGui::TreePop();
		}
	}

	void mesh_manager::on_register(std::shared_ptr<manager_host> host)
	{
		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');	
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
