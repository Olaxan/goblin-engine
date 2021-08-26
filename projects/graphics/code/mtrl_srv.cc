#include "mtrl_srv.h"

#include <GL/glew.h>

namespace efiilj
{
	material_server::material_server()
	{
		printf("Init materials...\n");
	}

	material_server::~material_server()
	{
		printf("Material server exit\n");
	}

	std::string get_texture_type_name(const texture_type& tex)
	{
		switch (tex)
		{
			case texture_type::tex_base:
				return "tex_base";
			case texture_type::tex_normal:
				return "tex_normal";
			case texture_type::tex_orm:
				return "tex_orm";
			case texture_type::tex_emissive:
				return "tex_emissive";
			case texture_type::tex_framebuffer:
				return "tex_framebuffer";
			default:
				return "tex_default";
		}
	}

	void material_server::append_defaults(material_id)
	{
		_data.shader.emplace_back(-1);
		_data.textures.emplace_back();
		_data.base_color.emplace_back(vector4(1,1,1,1));
		_data.emissive_factor.emplace_back(0);
		_data.metallic_factor.emplace_back(1.0f);
		_data.roughness_factor.emplace_back(0.5f);
		_data.alpha_cutoff.emplace_back(0.1f);
		_data.double_sided.emplace_back(false);
	}

	void material_server::on_register(std::shared_ptr<manager_host> host)
	{
		_shaders = host->get_manager_from_fcc<shader_server>('SHDR');
		_textures = host->get_manager_from_fcc<texture_server>('TXSR');
	}

	bool material_server::apply(material_id idx)
	{
		if (!is_valid(idx))
			return false;

		shader_id sid = _data.shader[idx];

		if (!_shaders->use(sid))
			return false;

		_shaders->set_uniform(sid, "base_color_factor", _data.base_color[idx]);

		for (const auto& tex : _data.textures[idx])
		{
			texture_type type = _textures->get_type(tex);
			unsigned slot = static_cast<unsigned>(type);
			_textures->set_active(tex, slot);
			_shaders->set_uniform(sid, get_texture_type_name(type), static_cast<int>(slot));
		}

		if (_data.double_sided[idx])
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);

		return true;
	}

	void material_server::add_texture(material_id idx, texture_id tex_id)
	{
		_data.textures[idx].emplace_back(tex_id);
	}

	void material_server::set_base_color(material_id idx, const vector4& base)
	{
		_data.base_color[idx] = base;
	}

	void material_server::set_emissive_factor(material_id idx, const vector3& emit)
	{
		_data.emissive_factor[idx] = vector4(emit, 1.0f);
	}

	void material_server::set_metallic_factor(material_id idx, const float& factor)
	{
		_data.metallic_factor[idx] = factor;
	}

	void material_server::set_roughness_factor(material_id idx, const float& factor)
	{
		_data.roughness_factor[idx] = factor;
	}

	void material_server::set_alpha_cutoff(material_id idx, const float& cutoff)
	{
		_data.alpha_cutoff[idx] = cutoff;
	}

	void material_server::set_double_sided(material_id idx, bool double_sided)
	{
		_data.double_sided[idx] = double_sided;
	}
}
