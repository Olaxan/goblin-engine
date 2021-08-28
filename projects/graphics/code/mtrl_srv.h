#pragma once

#include "server.h"
#include "mgr_host.h"
#include "tex_srv.h"
#include "shdr_mgr.h"

#include <memory>

namespace efiilj
{
	typedef int material_id;

	class material_server : public server<material_id>
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

			std::shared_ptr<shader_server> _shaders;
			std::shared_ptr<texture_server> _textures;
			
		public:

			material_server();
			~material_server();

			void append_defaults(material_id) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			void draw_gui(material_id) override;

			bool apply(material_id idx, shader_id fallback = -1);
			void add_texture(material_id idx, texture_id tex_id);

			const std::vector<texture_id>& get_textures(material_id idx)
			{
				return _data.textures[idx];
			}

			shader_id get_program(material_id idx) const
			{
				return _data.shader[idx];
			}

			void set_program(material_id idx, shader_id prog)
			{
				_data.shader[idx] = prog;
			}

			void set_base_color(material_id idx, const vector4& base);
			void set_emissive_factor(material_id idx, const vector3& emit);
			void set_metallic_factor(material_id idx, const float& factor);
			void set_roughness_factor(material_id idx, const float& factor);
			void set_alpha_cutoff(material_id idx, const float& cutoff);
			void set_double_sided(material_id idx, bool double_sided);

	};
}
