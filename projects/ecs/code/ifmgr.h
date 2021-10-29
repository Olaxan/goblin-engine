#pragma once

#include "eid.h"
#include "msg.h"
#include <memory>
#include <string>

namespace efiilj
{
	class manager_host;
	class registrable
	{
		public:

			virtual void init() { };

			virtual void on_register(std::shared_ptr<manager_host>) { };
			virtual void on_setup() { };
			virtual void on_begin_frame() { };
			virtual void on_frame() { };
			virtual void on_end_frame() { };
	};

	class server_base : public registrable
	{

	};

	class component_base : public registrable
	{
		public:

			virtual void set_dispatcher(std::shared_ptr<manager_host>) { }
			virtual void draw_entity_gui(entity_id eid) = 0;
			virtual void register_from_editor(entity_id eid) = 0;

			virtual const std::string& get_component_name() const = 0;

			virtual void on_editor_gui() { };
			virtual void on_message(message_type, entity_id) { };
			virtual void on_validate(entity_id) { };

	};
}
