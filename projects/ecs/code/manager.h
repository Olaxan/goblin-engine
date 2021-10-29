#pragma once

#include "entity.h"
#include "mgr_host.h"
#include "comp.h"
#include "mgrdata.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <set>

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

			std::unordered_map<entity_id, std::set<T>> _instance_mapping;

			struct
			{
				ComponentData<T> instances;
				ComponentData<entity_id> entities;
				ComponentData<bool> enabled { true };
				ComponentData<bool> alive { true };
				ComponentData<bool> error { false };
			} _com;

			size_t count = 0;

			std::vector<Extensible*> _ds;

			std::string _name;

			virtual void on_activate(T) { }
			virtual void on_deactivate(T) { }
			virtual void on_validate(T) { }
			virtual void on_destroy(T) { }
			virtual void on_message(message_type, entity_id) { }

			virtual void on_editor_gui() override {}
			virtual void on_editor_gui(T) {};

			void pack_data(int to, int from)
			{
				for (Extensible*& data : _ds)
					data->pack(to, from);
			}

			void extend_data()
			{
				for (Extensible*& data : _ds)
					data->extend();
			}

			void trim_data()
			{
				for (Extensible*& data : _ds)
					data->trim();
			}

			void add_data(Extensible* data)
			{
				_ds.emplace_back(data);
			}

			void add_data( std::initializer_list<Extensible*> list )
			{
				for( auto data : list )
					add_data(data);
			}

			void init() override
			{
				add_data({
						&_com.instances,
						&_com.entities,
						&_com.enabled,
						&_com.alive,
						&_com.error});
			}

		public:

			void set_dispatcher(std::shared_ptr<manager_host> dispatcher) override
			{
				_dispatcher = std::move(dispatcher);
			}

			T register_entity(entity_id eid)
			{
				size_t free = _com.instances.size();

				T new_id = static_cast<T>(count);

				// If we need the space, extend vectors
				if (free <= count)
					extend_data();

				_com.entities[new_id] = eid;
				_com.instances[new_id] = new_id;

				// Add entity id to instance map
				_instance_mapping[eid].emplace(new_id);

				count++;

				on_activate(new_id);

				return new_id;
			}

			bool unregister_entity(entity_id eid)
			{
				auto components = get_components(eid);

				for (auto idx : components)
					remove_component(idx);
			}

			void remove_component(T idx)
			{
				entity_id eid = get_entity(idx);
				on_deactivate(idx);
				destroy(idx);
				_dispatcher->validate(eid);
			}

			void reset_component(T idx)
			{
				for (Extensible*& data : _ds)
					data->reset_default(idx);
			}

			void destroy(T idx)
			{
				on_destroy(idx);

				T last = _com.instances[count - 1];

				entity_id eid = get_entity(idx);
				entity_id last_eid = get_entity(last);

				if (idx != last)
				{
					pack_data(idx, last);
					_instance_mapping[last_eid].erase(last);
					_instance_mapping[last_eid].emplace(idx);
				}

				_instance_mapping[eid].erase(idx);

				trim_data();
				count--;
			}

			void register_from_editor(entity_id eid) override
			{
				register_entity(eid);
			}

			void draw_entity_gui(entity_id eid) override
			{
				const auto& components = get_components(eid);

				for (auto idx : components)
				{
					ImGui::PushID(idx);

					if (ImGui::TreeNode(get_component_name().c_str()))
					{
						bool enabled = _com.enabled[idx];
						if (ImGui::Checkbox("", &enabled))
							_com.enabled[idx] = enabled;

						ImGui::SameLine();

						if (ImGui::SmallButton("Reset"))
							reset_component(idx);

						ImGui::SameLine();

						if (ImGui::SmallButton("Remove"))
						{
							remove_component(idx);
							ImGui::TreePop();
							ImGui::PopID();
							return;
						}

						ImGui::Spacing();

						if (get_error(idx))
						{
							ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error: [ERROR_MESSAGE]");
						}
						else
							on_editor_gui(idx);

						ImGui::TreePop();
					}

					ImGui::PopID();
					ImGui::Separator();
				}
			}
			
			virtual bool is_valid(T idx) const
			{ return (idx >= 0 && idx < static_cast<int>(count)); }

			inline const std::string& get_component_name() const override
			{ return _name; }

			inline const entity_id& get_entity(T idx) const
			{ return _com.entities[idx]; }

			inline bool get_error(T idx) const
			{ return _com.error[idx]; }

			inline void set_error(T idx, bool error = true)
			{ _com.error[idx] = error; }

			inline bool get_enabled(T idx) const
			{ return _com.enabled[idx]; }

			inline void set_enabled(T idx, bool enabled = true)
			{ _com.enabled[idx] = enabled; }

			inline bool get_alive(T idx) const
			{ return _com.alive[idx]; }

			const std::vector<T>& get_instances() const
			{ return _com.instances.data(); }

			const std::set<T>& get_components(entity_id eid)
			{ return _instance_mapping[eid]; }

			T get_component(entity_id eid)
			{
				const auto& components = _instance_mapping[eid];
				return (components.size() > 0) ? *components.begin() : -1;
			}
	};
}
