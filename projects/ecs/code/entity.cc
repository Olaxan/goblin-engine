#include "entity.h"

namespace efiilj
{
	entity_manager::entity_manager()
	{}

	entity_manager::~entity_manager()
	{
		//printf("Entity manager exit\n");
	}

	entity_id entity_manager::create_entity()
	{
		entity_id e = _entities.size() + 1;
		_entities.emplace_back(e);
		return e;
	}

	bool entity_manager::destroy_entity(entity_id eid)
	{
		return false;
	}
}
