#include "script_mgr.h"

#include <pybind11/pybind11.h>

namespace efiilj
{
	script_manager::script_manager()
	{
		printf("Init script manager...\n");
		//_name = "Script";
	}

	script_manager::~script_manager()
	{
		printf("Script manager exit\n");
	}

	void script_manager::on_setup()
	{
		py::print("Hello from Python!");
	}

	void script_manager::on_register(std::shared_ptr<manager_host>)
	{
		add_data(&_data.path);
	}
}
