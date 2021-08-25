#pragma once

#include "manager.h"
#include "mgr_host.h"

#include "mtrl_srv.h"

namespace efiilj
{

	typedef int material_instance_id;

	class material_manager : public manager<material_instance_id>, public registrable
	{
		private:

			struct MaterialInstanceData
			{
				std::vector<material_id> id;
			} _data;

		public:

			material_manager();
			~material_manager();

			material_instance_id register_entity(entity_id eid) override;
			bool unregister_entity(material_instance_id idx) override;

			void draw_gui() override;
			void draw_gui(material_instance_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			material_id get_material(material_instance_id idx);
			void set_material(material_instance_id idx, material_id mat_id);
	};
}
