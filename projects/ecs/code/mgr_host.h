#pragma once

#include "manager.h"
#include "ifmgr.h"

#include <memory>
#include <unordered_map>

namespace efiilj
{

	union fourcc
	{
		int code;
		char name[4];
	};

	class manager_host
	{
		private:

			std::unordered_map<int, std::shared_ptr<registrable>> _reg;

		public:

			manager_host();
			~manager_host();

			void register_manager(std::shared_ptr<registrable> mgr, int fcc);
	};
}
