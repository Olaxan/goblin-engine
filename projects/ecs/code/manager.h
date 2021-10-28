#pragma once

#include "entity.h"
#include "mgr_host.h"
#include "comp.h"
#include "mgrdata.h"

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

			std::shared_ptr<manager_host> _dispatcher;

			std::multimap<entity_id, T> _instance_mapping;

			size_t count = 0;

			struct
			{
				std::vector<T> instances;
				std::vector<entity_id> entities;
				std::vector<bool> enabled;
				std::vector<bool> alive;
				std::vector<bool> error;
			} _com;

			std::vector<Extensible*> _ds;

			std::string _name;

			virtual void on_activate(T) { }
			virtual void on_deactivate(T) { }
			virtual void on_validate(T) { }
			virtual void on_destroy(T) { }

			virtual void on_editor_gui() override {}
			virtual void on_editor_gui(T) {};

			void pack_com(T to, T from)
			{
				_com.instances[to] = _com.instances[from];	
				_com.entitites[to] = _com.entitites[from];	
				_com.enabled[to] = _com.enabled[from];	
				_com.alive[to] = _com.alive[from];	
				_com.error[to] = _com.error[from];	
			}

			void extend_com(T idx, entity_id eid)
			{
				_com.instances.emplace_back(idx);
				_com.entities.emplace_back(eid);
				_com.enabled.emplace_back(true);
				_com.alive.emplace_back(true);
				_com.error.emplace_back(false);
			}

			void pack_data(size_t to, size_t from)
			{
				for (Extensible*& data : _ds)
				{
					data->pack(to, from);
				}
			}

			void extend_data()
			{
				for (Extensible*& data : _ds)
				{
					data->extend();
				}
			}

			void add_data(Extensible* data)
			{
				_ds.emplace_back(data);
			}

			void add_data(Extensible* data, Extensible* args...)
			{
				add_data(data);
				add_data(args);
			}

			void add_data( std::initializer_list<Extensible*> list )
			{
				for( auto data : list )
				{
					add_data(data);
				}
			}

		public:

			void set_dispatcher(std::shared_ptr<manager_host> dispatcher)
			{
				_dispatcher = std::move(dispatcher);
			}

			T register_entity(entity_id eid)
			{
				size_t free = _com.instances.size();

				T new_id = static_cast<T>(count);

				// If we need the space, extend vectors
				if (free <= count)
				{
					extend_com(new_id, eid);
					extend_data();
				}

				// Add entity id to instance map
				_instance_mapping.emplace(eid, new_id);

				count++;

				on_activate(new_id);

				return new_id;
			}

			bool unregister_entity(entity_id eid)
			{
				auto range = get_components(eid);

				for (auto it = range.first; it != range.second; it++)
					remove_component(it->second);
			}

			bool remove_component(T idx)
			{
				_com.alive[idx] = false;
				on_deactivate(idx);
			}

			void destroy(T idx)
			{
				on_destroy(idx);

				T last = _com.instances.back();	
				pack_com(idx, last);
				pack_data(idx, last);

				_instance_mapping[idx] = _instance_mapping[last];
				_instance_mapping.erase(last);

				count--;
			}

			void register_from_editor(entity_id eid) override
			{
				register_entity(eid);
			}

			void draw_entity_gui(entity_id eid) override
			{
				auto range = get_components(eid);

				for (auto it = range.first; it != range.second; it++)
				{
					T idx = it->second;

					ImGui::PushID(idx);

					if (ImGui::TreeNode(get_name().c_str()))
					{
						bool enabled = _com.enabled[idx];
						if (ImGui::Checkbox("Enabled", &enabled))
							_com.enabled[idx] = enabled;

						ImGui::SameLine();

						if (ImGui::SmallButton("Remove"))
							remove_component(idx);

						if (get_error(idx))
						{
							ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error: [ERROR_MESSAGE]");
						}
						else
							on_editor_gui(idx);

						ImGui::TreePop();
					}

					ImGui::SameLine();

					ImGui::PopID();
					ImGui::Separator();
				}
			}
			
			virtual bool is_valid(T idx) const
			{
				return (idx >= 0 && idx < static_cast<int>(_com.instances.size()));
			}

			inline const std::string& get_name() const override
			{ return _name; }

			inline const entity_id& get_entity(T idx) const
			{ return _com.entities[idx]; }

			inline bool get_error(T idx) const
			{ return _com.error[idx]; }

			inline bool get_enabled(T idx) const
			{ return _com.enabled[idx]; }

			inline bool get_alive(T idx) const
			{ return _com.alive[idx]; }

			const std::vector<T>& get_instances() const
			{ return _com.instances; }

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
