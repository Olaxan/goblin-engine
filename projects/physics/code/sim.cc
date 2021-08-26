#include "sim.h"

namespace efiilj
{

	simulator::simulator()
	{
		printf("Init simulator...\n");
	}

	simulator::~simulator()
	{}

	void simulator::extend_defaults(physics_id)
	{}

	void simulator::draw_gui()
	{}

	void simulator::draw_gui(physics_id)
	{}
	
	void simulator::on_register(std::shared_ptr<manager_host> host)
	{}
}
