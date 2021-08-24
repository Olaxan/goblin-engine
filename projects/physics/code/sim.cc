#include "sim.h"

namespace efiilj
{

	simulator::simulator()
	{
		printf("Init simulator...\n");
	}

	simulator::~simulator()
	{}

	physics_id simulator::register_entity(entity_id eid)
	{
		return 0;
	}

	bool simulator::unregister_entity(physics_id idx)
	{
		return false;
	}

	void simulator::draw_gui()
	{}

	void simulator::draw_gui(physics_id)
	{}
	
	void simulator::on_register(std::shared_ptr<manager_host> host)
	{}
}
