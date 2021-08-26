#include "tex_srv.h"
#include "stb_image.h"

#include "GL/glew.h"
#include "iostream"

namespace efiilj
{
	texture_server::texture_server()
	{
		printf("Init textures...\n");
	}

	texture_server::~texture_server()
	{
		printf("Texture server exit\n");
	}

	void texture_server::append_defaults(texture_id)
	{
		_data.uri.emplace_back();
		_data.name.emplace_back();
		_data.usage.emplace_back(texture_type::tex_default);
		_data.tex_id.emplace_back(0);
		_data.state.emplace_back(false);
		_data.tex_wrap_s.emplace_back(GL_REPEAT);
		_data.tex_wrap_t.emplace_back(GL_REPEAT);
		_data.tex_min_filter.emplace_back(GL_LINEAR_MIPMAP_LINEAR);
		_data.tex_mag_filter.emplace_back(GL_LINEAR);
		_data.tex_format.emplace_back(GL_RGBA);
		_data.tex_type.emplace_back(GL_UNSIGNED_BYTE);
		_data.width.emplace_back(0);
		_data.height.emplace_back(0);
		_data.bits.emplace_back(0);
	}

	bool texture_server::load(texture_id idx)
	{
		if (_data.state[idx])
			return true;

		generate(idx);
		set_params(idx);

		stbi_set_flip_vertically_on_load(true);

		unsigned char* buf = stbi_load(
				_data.uri[idx].c_str(), &_data.width[idx], &_data.height[idx], &_data.bits[idx], 4);

		stbi_set_flip_vertically_on_load(false);

		if (buf == nullptr)
			return false;

		buffer(idx, _data.width[idx], _data.height[idx], buf);

		stbi_image_free(buf);

		unbind();
		
		return true;	
	}

	void texture_server::bind(texture_id idx) const
	{
		glBindTexture(GL_TEXTURE_2D, _data.tex_id[idx]);
	}
	
	void texture_server::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void texture_server::set_active(texture_id idx, unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		bind(idx);
	}

	void texture_server::generate(texture_id idx)
	{
		glGenTextures(1, &_data.tex_id[idx]);
	}

	void texture_server::buffer(texture_id idx, const unsigned int& width, const unsigned int& height, void* data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 
				width, height, 0, _data.tex_format[idx], _data.tex_type[idx], data);

		glGenerateMipmap(GL_TEXTURE_2D);

		_data.width[idx] = width;
		_data.height[idx] = height;
		_data.state[idx] = true;
	}

	void texture_server::set_params(texture_id idx)
	{
		glBindTexture(GL_TEXTURE_2D, _data.tex_id[idx]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _data.tex_wrap_s[idx]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _data.tex_wrap_t[idx]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _data.tex_min_filter[idx]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _data.tex_mag_filter[idx]);
	}

	const std::filesystem::path& texture_server::get_uri(texture_id idx) const
	{
		return _data.uri[idx];
	}

	void texture_server::set_uri(texture_id idx, const std::filesystem::path& uri)
	{
		_data.uri[idx] = uri;
	}

	void texture_server::set_format(texture_id idx, unsigned int format)
	{
		_data.tex_format[idx] = format;
	}

	void texture_server::set_type(texture_id idx, unsigned int type)
	{
		_data.tex_type[idx] = type;
	}

	void texture_server::set_name(texture_id idx, const std::string& name)
	{
		_data.name[idx] = name;
	}

	void texture_server::set_usage(texture_id idx, const texture_type& usage)
	{
		_data.usage[idx] = usage;
	}
}
