#pragma once

#include <memory>

namespace efiilj
{
	class manager_host;
	class registrable
	{
		public:

			virtual void on_register(std::shared_ptr<manager_host> host) = 0;
	};
}
