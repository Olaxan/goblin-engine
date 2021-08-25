#pragma once

#include "server.h"
#include "mgr_host.h"

#include <vector>

namespace efiilj
{
	typedef unsigned int entity_id;

	class entity_manager : public server<entity_id>, public registrable
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
