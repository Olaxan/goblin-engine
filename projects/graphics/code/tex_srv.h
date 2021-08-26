#pragma once

#include "server.h"
#include "mgr_host.h"

#include <filesystem>

namespace efiilj
{
	typedef int texture_id;

	enum class texture_type
	{
		tex_default,
		tex_base,
		tex_orm,
		tex_normal,
		tex_emissive,
		tex_framebuffer,
	};

	class texture_server : public server<texture_id>
	{
		private:

			struct TextureData 
			{
				std::vector<unsigned int> tex_id;
				std::vector<std::filesystem::path> uri;
				std::vector<std::string> name;
				std::vector<texture_type> usage;
				std::vector<unsigned int> tex_wrap_s;
				std::vector<unsigned int> tex_wrap_t;
				std::vector<unsigned int> tex_min_filter;
				std::vector<unsigned int> tex_mag_filter;
				std::vector<unsigned int> tex_format;
				std::vector<unsigned int> tex_type;
				std::vector<int> width;
				std::vector<int> height;
				std::vector<int> bits;
				std::vector<bool> state;
			} _data;

		public:

			texture_server();
			~texture_server();

			void append_defaults(texture_id) override;

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
			void set_usage(texture_id idx, const texture_type& usage);
	};
}
