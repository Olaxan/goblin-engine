#pragma once

#include "entity.h"
#include "comp.h"

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

		public:

			entity_editor(std::shared_ptr<entity_manager> ent, std::shared_ptr<manager_host> mgr_host);
			~entity_editor();

			void get_managers();
			void show_entity_gui();
	};
}
