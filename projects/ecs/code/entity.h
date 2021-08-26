#pragma once

#include "eid.h"
#include "server.h"
#include "mgr_host.h"

#include <vector>

namespace efiilj
{
	class entity_manager : public server<entity_id>
	{
		private:

		public:

			entity_manager();
			~entity_manager();

			void append_defaults(entity_id) override;
			
	};
}
