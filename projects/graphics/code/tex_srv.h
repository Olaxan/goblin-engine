#pragma once

#include "server.h"
#include "mgr_host.h"

#include <filesystem>

namespace efiilj
{
	typedef int texture_id;

	enum class texture_type
	{
		tex_base = 0,
		tex_normal = 1,
		tex_orm = 2,
		tex_emissive = 3,
		tex_framebuffer = 4,
		tex_default = 5,
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
			void unbind() const;
			void set_active(texture_id idx, unsigned int slot) const;
			void generate(texture_id idx);
			void buffer(texture_id idx, const unsigned int& width, const unsigned int& height, void* data);
			void set_params(texture_id idx);

			const std::filesystem::path& get_uri(texture_id idx) const;
			void set_uri(texture_id idx, const std::filesystem::path& uri);

			void set_format(texture_id idx, unsigned int format);

			const texture_type& get_type(texture_id idx) const
			{
				return _data.usage[idx];
			}

			void set_type(texture_id idx, unsigned int type);
			void set_name(texture_id idx, const std::string& name);
			void set_usage(texture_id idx, const texture_type& usage);
	};
}
