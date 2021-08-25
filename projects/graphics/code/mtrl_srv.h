#pragma once

#include "server.h"
#include "mgr_host.h"
#include "tex_srv.h"

#include "shdr_mgr.h"

namespace efiilj
{
	typedef int material_id;

	class material_server : public server<material_id>, public registrable
	{
		private:

			struct MaterialData
			{
				std::vector<shader_id> shader;
				std::vector<std::vector<texture_id>> textures;

				std::vector<vector4> base_color;
				std::vector<vector4> emissive_factor;
				std::vector<float> metallic_factor;
				std::vector<float> roughness_factor;
				std::vector<float> alpha_cutoff;

				std::vector<bool> double_sided;
			} _data;
			
		public:

			material_server();
			~material_server();

			material_id create() override;
			bool destroy(material_id idx) override;

			void add_texture(material_id idx, texture_id tex_id);

			void set_base_color(material_id idx, const vector4& base);
			void set_emissive_factor(material_id idx, const vector3& emit);
			void set_metallic_factor(material_id idx, const float& factor);
			void set_roughness_factor(material_id idx, const float& factor);
			void set_alpha_cutoff(material_id idx, const float& cutoff);
			void set_double_sided(material_id idx, bool double_sided);

	};
}
