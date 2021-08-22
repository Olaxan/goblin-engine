#pragma once

#include "entity.h"

#include <vector>
#include <unordered_map>

namespace efiilj
{
	template<class T>
	class manager
	{

		std::vector<T> _instances;
		std::unordered_map<entity_id, std::vector<T>> _instance_mapping;

		virtual T register_entity(entity_id eid) = 0;
		virtual bool unregister_entity(T idx) = 0;
	};
}
