#include "mgr_host.h"

#include "stdio.h"
#include <memory>

namespace efiilj
{
	manager_host::manager_host()
	{
		printf("Init manager host...\n");
	}

	manager_host::~manager_host()
	{
		printf("Manager host exit\n");
	}

	void manager_host::register_manager(std::shared_ptr<registrable> mgr, int fcc)
	{
		mgr->on_register(shared_from_this());
		_reg.emplace(fcc, std::move(mgr));

		fourcc f = { fcc };
		printf("Registered manager %c%c%c%c\n", f.d, f.c, f.b, f.a);
	}
}
