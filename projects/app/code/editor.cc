#include "editor.h"

#include "imgui.h"

namespace efiilj
{
	entity_editor::entity_editor(std::shared_ptr<entity_manager> ents, std::shared_ptr<manager_host> mgr_host)
		: _selected_entity(-1), _mgr_host(std::move(mgr_host)), _entities(std::move(ents))
	{
		get_managers();	
	}

	entity_editor::~entity_editor()
	{
		printf("Editor exit\n");
	}

	void entity_editor::show_entity_gui()
	{
		const auto& ids = _entities->get_ids();

		ImGui::BeginChild("Entities", ImVec2(150, 0), true);
		for (const auto& id : ids)
		{
			char label[128];
			sprintf(label, "Entity %d", id);
			if (ImGui::Selectable(label, _selected_entity == id))
				_selected_entity = id;
		}
		ImGui::EndChild();

        ImGui::SameLine();
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("Entity view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			for (const auto& mgr : _managers)
			{
				mgr->draw_entity_gui(_selected_entity);
			}
            ImGui::EndChild();
            ImGui::EndGroup();
        }
	}

	void entity_editor::get_managers()
	{
		_managers = _mgr_host->get_components();
	}
}
