#pragma once

#include "entity.h"
#include "comp.h"
#include "meta_mgr.h"
#include "trfm_mgr.h"

#include <vector>

namespace efiilj
{
	class entity_editor
	{
		private:

			int _selection_mask = (1 << 2);
			ImGuiTreeNodeFlags _base_flags = 
				ImGuiTreeNodeFlags_OpenOnArrow 
				| ImGuiTreeNodeFlags_OpenOnDoubleClick;

			entity_id _selected_entity;
			std::shared_ptr<manager_host> _mgr_host;
			std::shared_ptr<entity_manager> _entities;
			std::vector<std::shared_ptr<component_base>> _managers;

			std::shared_ptr<meta_manager> _metadata;
			std::shared_ptr<transform_manager> _transforms;

			void draw_node(entity_id id, int depth, int& node_clicked);
			void draw_add_component();

			bool _is_add_comp_open = false;

		public:

			entity_editor(std::shared_ptr<entity_manager> ent, std::shared_ptr<manager_host> mgr_host);
			~entity_editor();

			void show_entity_gui();
			void setup();

			void set_selected(entity_id eid)
			{
				if (!_entities->is_valid(eid))
					return;

				_selected_entity = eid;
				_selection_mask = (1 << eid);
			}
	};
}
