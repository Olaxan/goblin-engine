#include "tex_srv.h"
#include "stb_image.h"

#include "GL/glew.h"
#include "iostream"

namespace efiilj
{
	texture_server::texture_server()
	{
		printf("Init textures...\n");
		stbi_set_flip_vertically_on_load(true);
	}

	texture_server::~texture_server()
	{
		printf("Texture server exit\n");
	}

	texture_id texture_server::create()
	{
		texture_id new_id = _pool.size();
		_pool.emplace_back(new_id);

		_data.uri.emplace_back();
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

		return new_id;
	}

	bool texture_server::destroy(texture_id idx)
	{
		return false;
	}

	bool texture_server::load(texture_id idx)
	{
		if (_data.state[idx])
			return true;

		glGenTextures(1, &_data.tex_id[idx]);
		glBindTexture(GL_TEXTURE_2D, _data.tex_id[idx]);

		set_params(idx);
		buffer(idx);

		return true;	
	}

	bool texture_server::bind(texture_id idx, unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, _data.tex_id[idx]);
	}

	bool texture_server::buffer(texture_id idx)
	{
		unsigned char* buffer = stbi_load(
				_data.uri[idx].c_str(), &_data.width[idx], &_data.height[idx], &_data.bits[idx], 4);

		if (buffer == nullptr)
			return false;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 
				_data.width[idx], _data.height[idx], 0, _data.tex_format[idx], _data.tex_type[idx], buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(buffer);

		_data.state[idx] = true;

		return true;
	}

	bool texture_server::set_params(texture_id idx)
	{
		glBindTexture(GL_TEXTURE_2D, _data.tex_id[idx]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _data.tex_wrap_s[idx]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _data.tex_wrap_t[idx]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _data.tex_min_filter[idx]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _data.tex_mag_filter[idx]);

		return true;
	}

	const std::filesystem::path& texture_server::get_uri(shader_id idx) const
	{
		return _data.uri[idx];
	}

	void texture_server::set_uri(shader_id idx, const std::filesystem::path& uri)
	{
		_data.uri[idx] = uri;
	}
}
