#pragma once

#include "server.h"

#include <vector>

namespace efiilj
{
	typedef unsigned int entity_id;

	class entity_manager : server<entity_id>
	{
		private:

			std::vector<entity_id> _entities;

		public:

			entity_manager();
			~entity_manager();

			entity_id create() override;
			bool destroy(entity_id eid) override;
	};
}
