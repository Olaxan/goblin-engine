#pragma once

#include "manager.h"
#include "ifmgr.h"

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
				ComponentData<light_type> type;
				ComponentData<light_base> base;
				ComponentData<attenuation_data> attenuation;
				ComponentData<cutoff_data> cutoff;
				ComponentData<transform_id> transform;
			} _data;

			std::shared_ptr<transform_manager> _transforms;

		public:

			light_manager();
			~light_manager();

			void on_editor_gui() override;
			void on_editor_gui(light_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;
			void on_validate(entity_id) override;

			void on_activate(light_id) override;

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
