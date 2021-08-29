#pragma once

#include "entity.h"
#include "comp.h"

#include <vector>
#include <map>

#include "imgui.h"

#include <string>
#include <sstream>

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

			std::string _name;

			virtual void extend_defaults(T new_id) = 0;

		public:

			virtual T register_entity(entity_id eid)
			{
				T new_id = static_cast<T>(_instances.size());

				_instances.emplace_back(new_id);
				_entities.emplace_back(eid);
				_alive.emplace_back(true);
				_instance_mapping.emplace(eid, new_id);

				extend_defaults(new_id);

				return new_id;
			}

			virtual bool unregister_entity(T idx)
			{
				_alive[idx] = false;
			}

			void register_from_editor(entity_id eid) override
			{
				register_entity(eid);
			}
			
			// Maybe improve in the future
			virtual void draw_gui() override {}

			virtual void draw_entity_gui(entity_id eid) override
			{
				auto range = get_components(eid);

				for (auto it = range.first; it != range.second; it++)
				{
					T idx = it->second;

					ImGui::PushID(idx);

					if (ImGui::TreeNode(get_name().c_str()))
					{
						draw_gui(idx);
						ImGui::TreePop();
					}

					ImGui::PopID();
					ImGui::Separator();
				}
			}

			virtual void draw_gui(T) {};

			const std::string& get_name() const override
			{
				return _name;
			}

			virtual bool is_valid(T idx) const
			{
				return (idx >= 0 && idx < static_cast<int>(_instances.size()));
			}

			const entity_id& get_entity(T idx) const
			{
				return _entities[idx];
			}

			const std::vector<T>& get_instances()
			{
				return _instances;
			}

			typename std::pair< 
				typename std::multimap<entity_id, T>::iterator, 
				typename std::multimap<entity_id, T>::iterator >
			get_components(entity_id eid)
			{
				return _instance_mapping.equal_range(eid);
			}

			T get_component(entity_id eid)
			{
				auto it = _instance_mapping.find(eid);
				return it == _instance_mapping.end() ? -1 : it->second;
			}
	};
}
