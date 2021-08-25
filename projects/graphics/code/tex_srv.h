#pragma once

#include "server.h"
#include "mgr_host.h"

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
				std::vector<std::string> name;
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

		public:

			texture_server();
			~texture_server();

			texture_id create() override;
			bool destroy(texture_id idx) override;

			bool load(texture_id idx);

			void bind(texture_id idx) const;
			void set_active(texture_id idx, unsigned int slot) const;
			void generate(texture_id idx);
			void buffer(texture_id idx, const unsigned int& width, const unsigned int& height, void* data);
			void set_params(texture_id idx);

			const std::filesystem::path& get_uri(texture_id idx) const;
			void set_uri(texture_id idx, const std::filesystem::path& uri);

			void set_format(texture_id idx, unsigned int format);
			void set_type(texture_id idx, unsigned int type);
			void set_name(texture_id idx, const std::string& name);
	};
}
