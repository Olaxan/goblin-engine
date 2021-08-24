#include "shdr_mgr.h"

namespace efiilj
{
	shader_server::shader_server()
	{
		printf("Init shaders...\n");
	}

	shader_server::~shader_server()
	{
		printf("Shader manager exit\n");
	}

	void shader_server::on_register(std::shared_ptr<manager_host> host)
	{
		
	}
}
