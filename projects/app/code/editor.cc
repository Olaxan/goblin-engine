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

	void entity_editor::draw_node(entity_id id, int depth, int& node_clicked)
	{

		std::stringstream ss;

		std::vector<transform_id> child_nodes;

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

		if (_transforms->is_valid(trf_id))
		{
			// Skip if invoked by main loop and we're a child
			// since we'll be drawn by our parent
			if (depth == 0 && _transforms->get_parent(trf_id) != -1)
				return;

			const auto& children = _transforms->get_children(trf_id);
			child_nodes.insert(child_nodes.end(), children.begin(), children.end());
		}
		else
			ss << " (virtual)";

		bool has_children = child_nodes.size() > 0;

		if (has_children)
			ss << " [" << child_nodes.size() << "]";
		else
			node_flags |= ImGuiTreeNodeFlags_Leaf;

		bool node_open = ImGui::TreeNodeEx(
				reinterpret_cast<void*>(id), node_flags, 
				"%s", ss.str().c_str());

		if (ImGui::IsItemClicked())
			node_clicked = id;

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("TRF_ENTITY", &trf_id, sizeof(transform_id));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TRF_ENTITY"))
			{
				transform_id drag_trf = *static_cast<const transform_id*>(payload->Data);
				_transforms->set_parent(drag_trf, trf_id);
			}
			ImGui::EndDragDropTarget();
		}

		if (node_open)
		{
			for (const auto& child : child_nodes)
			{
				entity_id child_eid = _transforms->get_entity(child);
				draw_node(child_eid, depth + 1, node_clicked);
			}

			ImGui::TreePop();
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
			{
				draw_node(id, 0, node_clicked);
			}

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

			static bool is_add_comp_open = false;

			if (_selected_entity != -1)
			{

				if (is_add_comp_open)
				{
					ImGui::BeginChild("Add component", ImVec2(0, 128), true);
					if (ImGui::SmallButton("Close"))
						is_add_comp_open = false;

					for (const auto& mgr : _managers)
					{
						if (ImGui::Selectable(mgr->get_name().c_str()))
						{
							mgr->register_from_editor(_selected_entity);
							is_add_comp_open = false;
						}
					}
					ImGui::EndChild();
				}
				else if (ImGui::Button("Add component"))
					is_add_comp_open = true;
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
