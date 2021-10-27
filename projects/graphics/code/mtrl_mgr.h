#pragma once

#include "manager.h"
#include "mgr_host.h"

#include "mtrl_srv.h"
#include <memory>

namespace efiilj
{

	typedef int material_instance_id;

	class material_manager : public manager<material_instance_id>
	{
		private:

			struct MaterialInstanceData
			{
				std::vector<material_id> id;
			} _data;

			std::shared_ptr<material_server> _materials;
			std::shared_ptr<texture_server> _textures;

		public:

			material_manager();
			~material_manager();

			void extend_defaults(material_instance_id new_id) override;
			void on_editor_gui() override;
			void on_editor_gui(material_instance_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			material_id get_material(material_instance_id idx);
			void set_material(material_instance_id idx, material_id mat_id);
	};
}
