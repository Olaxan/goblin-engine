#include "editor.h"

#include "imgui.h"

#include <sstream>

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

	bool entity_editor::draw_leaf(entity_id id, ImGuiTreeNodeFlags& node_flags, const std::string& name)
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf 
			| ImGuiTreeNodeFlags_NoTreePushOnOpen;

		ImGui::TreeNodeEx(reinterpret_cast<void*>(id), node_flags, "%s", name.c_str()); 

		return ImGui::IsItemClicked();
	}

	void entity_editor::draw_node(entity_id id, int depth, int& node_clicked)
	{

		std::stringstream ss;

		ImGuiTreeNodeFlags node_flags = _base_flags;
		const bool is_selected = (_selection_mask & (1 << id)) != 0;

		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		meta_id met_id = _metadata->get_component(id);
		transform_id trf_id = _transforms->get_component(id);

		if (_metadata->is_valid(met_id))
			ss <<  _metadata->get_name(met_id);
		else
			ss << "Entity " << id;

		if (!_transforms->is_valid(trf_id))
		{
			ss << " (virtual)";
			if (draw_leaf(id, node_flags, ss.str()))
				node_clicked = id;
			return;
		}

		// Skip if invoked by main loop and we're a child
		// since we'll be rendered by our parent later
		if (depth == 0 && _transforms->get_parent(trf_id) != -1)
			return;

		const auto& children = _transforms->get_children(trf_id);

		if (children.size() > 0) // we are tree
		{
			ss << " [" << children.size() << "]";

			bool node_open = ImGui::TreeNodeEx(
					reinterpret_cast<void*>(id), node_flags, 
					"%s", ss.str().c_str());

			if (ImGui::IsItemClicked())
				node_clicked = id;

			if (node_open)
			{
				for (const auto& child : children) //NOLINT
				{
					entity_id child_eid = _transforms->get_entity(child);
					draw_node(child_eid, depth + 1, node_clicked);
				}

				ImGui::TreePop();
			}
		}
		else
		{
			if (draw_leaf(id, node_flags, ss.str()))
				node_clicked = id;
			return;
		}
	}

	void entity_editor::show_entity_gui()
	{
		const auto& ids = _entities->get_ids();

		ImGui::BeginChild("Entities", ImVec2(250, 0), true);
		if (ImGui::TreeNode("Scene root"))
		{

			int node_clicked = -1;

			for (const auto& id : ids)
				draw_node(id, 0, node_clicked);

			if (node_clicked != -1)
			{
				_selection_mask = (1 << node_clicked);
				_selected_entity = node_clicked;
			}

			ImGui::TreePop();
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
		_metadata = _mgr_host->get_manager_from_fcc<meta_manager>('META');
		_transforms = _mgr_host->get_manager_from_fcc<transform_manager>('TRFM');
	}
}
