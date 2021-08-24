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

	entity_id entity_manager::create()
	{
		entity_id e = _entities.size();
		_entities.emplace_back(e);

		printf("Created entity %d\n", e);

		return e;
	}

	bool entity_manager::destroy(entity_id eid)
	{
		printf("Destroying entity %d...\n", eid);

		return false;
	}
}
