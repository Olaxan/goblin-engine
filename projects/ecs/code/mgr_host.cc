#include "mgr_host.h"

#include "stdio.h"
#include <memory>

namespace efiilj
{
	manager_host::manager_host()
	{}

	manager_host::~manager_host()
	{
		printf("Manager host exit");
	}

	void manager_host::register_manager(std::shared_ptr<registrable> mgr, int fcc)
	{
		_reg.emplace(fcc, std::move(mgr));
		fourcc f = { fcc };
		printf("Registered manager %s\n", f.name);
	}
}
