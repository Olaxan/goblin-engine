#pragma once

#include <vector>

namespace efiilj
{
	typedef unsigned int entity_id;

	class entity_manager
	{
		private:

			std::vector<entity_id> _entities;

		public:

			entity_manager();
			~entity_manager();

			entity_id create_entity();
			bool destroy_entity(entity_id eid);
	};
}
