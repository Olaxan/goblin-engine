#pragma once

#include "entity.h"
#include "comp.h"

#include <vector>
#include <map>

#include "imgui.h"

namespace efiilj
{
	template<class T>
	class manager : public component_base
	{
		protected:

			std::multimap<entity_id, T> _instance_mapping;

			std::vector<T> _instances;
			std::vector<entity_id> _entities;
			std::vector<bool> _alive;

			virtual void extend_defaults(T new_id) = 0;

		public:

			virtual T register_entity(entity_id eid)
			{
				T new_id = static_cast<T>(_instances.size());

				_instances.emplace_back(new_id);
				_entities.emplace_back(eid);
				_alive.emplace_back(true);
				_instance_mapping.emplace(eid, new_id);

				extend_defaults(eid);

				return new_id;
			}

			virtual bool unregister_entity(T idx)
			{
				_alive[idx] = false;
			}
			
			// Maybe improve in the future
			virtual void draw_gui() override {}

			virtual void draw_entity_gui(entity_id eid) override
			{
				for (auto it = get_components_begin(eid); it != get_components_end(); it++)
				{
					draw_gui(it->second);
				}
			}

			virtual void draw_gui(T) {};

			virtual bool is_valid(T idx) const
			{
				return (idx >= 0 && idx < static_cast<int>(_instances.size()));
			}

			const std::vector<T>& get_instances()
			{
				return _instances;
			}

			typename std::multimap<entity_id, T>::iterator get_components_begin(entity_id eid)
			{
				return _instance_mapping.find(eid);
			}

			typename std::multimap<entity_id, T>::iterator get_components_end()
			{
				return _instance_mapping.end();
			}
	};
}
