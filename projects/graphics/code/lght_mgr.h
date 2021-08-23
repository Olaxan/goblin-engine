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
				std::vector<attenuation_data> attenuation;
				std::vector<cutoff_data> cutoff;
				std::vector<transform_id> transform;
			} _data;

			std::shared_ptr<transform_manager> _transforms;

		public:

			light_manager(std::shared_ptr<transform_manager> trf_mgr);
			~light_manager();

			light_id register_entity(entity_id eid) override;
			bool unregister_entity(light_id idx) override;
			void draw_gui() override;
			void draw_gui(light_id idx) override;

			void update_falloff(light_id idx);

			const light_type& get_type(light_id idx) const;
			void set_type(light_id idx, const light_type& type);

			const light_base& get_base(light_id idx) const;
			void set_base(light_id idx, const light_base& base);
			void set_base(light_id idx, const vector3& color, const float& ambient, const float& diffuse);

			const attenuation_data& get_attenuation(light_id idx) const;
			void set_attenuation(light_id idx, const attenuation_data& att);
			void set_attenuation(light_id idx, const float& constant, const float& linear, const float& exponential);
			const cutoff_data& get_cutoff(light_id idx) const;
			void set_cutoff(light_id idx, const cutoff_data& c);
			void set_cutoff(light_id idx, const float& inner, const float& outer);

			const transform_id& get_transform(light_id idx) const;
			void set_transform(light_id idx, const transform_id& trf_id);
	};
}
