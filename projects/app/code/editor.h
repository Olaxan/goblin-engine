#pragma once

#include "entity.h"
#include "comp.h"
#include "meta_mgr.h"

#include <vector>

namespace efiilj
{
	class entity_editor
	{
		private:

			entity_id _selected_entity;
			std::shared_ptr<manager_host> _mgr_host;
			std::shared_ptr<entity_manager> _entities;
			std::vector<std::shared_ptr<component_base>> _managers;

			std::shared_ptr<meta_manager> _metadata;
		public:

			entity_editor(std::shared_ptr<entity_manager> ent, std::shared_ptr<manager_host> mgr_host);
			~entity_editor();

			void get_managers();
			void show_entity_gui();

			void set_selected(entity_id eid)
			{
				if (_entities->is_valid(eid))
					_selected_entity = eid;
			}
	};
}
