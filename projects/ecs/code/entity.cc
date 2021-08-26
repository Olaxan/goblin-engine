#include "entity.h"

#include "stdio.h"

namespace efiilj
{
	entity_manager::entity_manager()
	{
		printf("Init entity manager...\n");
	}

	entity_manager::~entity_manager()
	{
		printf("Entity manager exit\n");
	}

	void entity_manager::append_defaults(entity_id)
	{

	}
}
