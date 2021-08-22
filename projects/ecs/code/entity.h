#pragma once

#include <vector>

namespace efiilj
{
	typedef unsigned int entity_id;

	template<class T>
	class manager
	{
		virtual T register_entity(entity_id eid) = 0;
		virtual bool unregister_entity(T idx) = 0;
	};

	class entity_manager
	{
		private:

			std::vector<entity_id> _entities;

		public:

			entity_manager();
			~entity_manager();

			entity_id create_entity();
			bool destroy_entity(entity_id eid);
	};
}
