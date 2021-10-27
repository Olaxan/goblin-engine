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

		fourcc f = { fcc };
		printf("Registered manager %c%c%c%c ", f.d, f.c, f.b, f.a);

		if (std::shared_ptr<component_base> cmp = std::dynamic_pointer_cast<component_base>(mgr))
		{
			_components.emplace_back(cmp);
			printf("[COMPONENT]");
		}

		if (std::shared_ptr<server_base> srv = std::dynamic_pointer_cast<server_base>(mgr))
		{
			_servers.emplace_back(srv);
			printf("[SERVER]");
		}

		_reg.emplace(fcc, std::move(mgr));
		printf("\n");
	}

	void manager_host::on_setup()
	{
		for (const auto& srv : _servers)
			srv->on_setup();

		for (const auto& com : _components)
			com->on_setup();
	}

	void manager_host::on_begin_frame()
	{
		for (const auto& srv : _servers)
			srv->on_begin_frame();

		for (const auto& com : _components)
			com->on_begin_frame();
	}

	void manager_host::on_frame()
	{
		for (const auto& srv : _servers)
			srv->on_frame();

		for (const auto& com : _components)
			com->on_frame();
	}

	void manager_host::on_end_frame()
	{
		for (const auto& srv : _servers)
			srv->on_end_frame();

		for (const auto& com : _components)
			com->on_end_frame();
	}
}
