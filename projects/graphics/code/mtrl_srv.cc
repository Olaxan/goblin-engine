#include "mtrl_srv.h"

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
