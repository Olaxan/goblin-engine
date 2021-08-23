#pragma once

#include "entity.h"

#include <vector>
#include <map>

#include "imgui.h"

namespace efiilj
{
	template<class T>
	class manager
	{
		protected:

			std::vector<T> _instances;
			std::multimap<entity_id, T> _instance_mapping;

			void map_new_element(entity_id, T);

		public:

			virtual T register_entity(entity_id eid) = 0;
			virtual bool unregister_entity(T idx) = 0;

			virtual void draw_gui() = 0;
			virtual void draw_gui(T selected) = 0;

			const std::vector<T>& get_instances()
			{
				return _instances;
			}
	};
}
