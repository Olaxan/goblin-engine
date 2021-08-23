#pragma once

#include "manager.h"

#include "trfm_mgr.h"
#include "light.h"

#include <memory>
#include <vector>

namespace efiilj
{
	typedef int light_id;

	class light_manager : public manager<light_id>
	{
		private:

			struct LightData
			{
				std::vector<light_type> type;
				std::vector<light_base> base;
				std::vector<attenuation> attenuation;
				std::vector<cutoff> cutoff;
				std::vector<transform_id> transform;
			} _data;

		public:

			light_manager(std::shared_ptr<transform_manager> trf_mgr);
			~light_manager();

			light_id register_entity(entity_id eid) override;
			bool unregister_entity(light_id idx) override;
			void draw_gui() override;
			void draw_gui(light_id idx) override;

			void set_type(light_id idx, light_type type);

	};
}
