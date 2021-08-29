#pragma once

#include "eid.h"
#include <memory>
#include <string>

namespace efiilj
{
	class manager_host;
	class registrable
	{
		public:

			virtual void on_register(std::shared_ptr<manager_host>) { };
	};

	class server_base : public registrable
	{

	};

	class component_base : public registrable
	{
		public:

			virtual void draw_gui() = 0;
			virtual void draw_entity_gui(entity_id eid) = 0;
			virtual void register_from_editor(entity_id eid) = 0;
			virtual const std::string& get_name() const = 0;
	};
}
