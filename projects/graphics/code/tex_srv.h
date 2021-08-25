#pragma once

#include "server.h"
#include "mgr_host.h"

#include "shdr_mgr.h"

#include <filesystem>

namespace efiilj
{
	typedef int texture_id;

	class texture_server : public server<texture_id>, public registrable
	{
		private:

			struct TextureData 
			{
				std::vector<unsigned int> tex_id;
				std::vector<std::filesystem::path> uri;
				std::vector<bool> state;
				std::vector<unsigned int> tex_wrap_s;
				std::vector<unsigned int> tex_wrap_t;
				std::vector<unsigned int> tex_min_filter;
				std::vector<unsigned int> tex_mag_filter;
				std::vector<unsigned int> tex_format;
				std::vector<unsigned int> tex_type;
				std::vector<int> width;
				std::vector<int> height;
				std::vector<int> bits;
			} _data;
			
			bool buffer(texture_id idx);
			bool set_params(texture_id idx);

		public:

			texture_server();
			~texture_server();

			texture_id create() override;
			bool destroy(texture_id idx) override;

			bool load(texture_id idx);
			bool bind(texture_id idx, unsigned int slot) const;

			const std::filesystem::path& get_uri(shader_id idx) const;
			void set_uri(shader_id idx, const std::filesystem::path& uri);

	};
}
