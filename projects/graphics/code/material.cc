#include "material.h"
#include <GL/glew.h>

namespace efiilj
{

	material_base::material_base(std::shared_ptr<shader_program> program)
		: program_(std::move(program)), double_sided(false), color(vector4(1.0f, 1.0f, 1.0f, 1.0f))
	{ }

	gltf_pbr_base::gltf_pbr_base(std::shared_ptr<shader_program> program)
			: material_base(program)
	{}

	void material_base::apply()
	{
		if (program_ == nullptr)
			return;

		if (!program_->use())
			return;

		if (double_sided)
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);

		program_->set_uniform("color", color);
		
	}

	void gltf_pbr_base::apply()
	{
		material_base::apply();

		program_->set_uniform("shininess", 128);

		auto it = textures_.find("BASE");
		if (it != textures_.end())
		{
			it->second->bind(0);
			program_->set_uniform("tex_base", 0);
		}

		it = textures_.find("NORMAL");
		if (it != textures_.end())
		{
			it->second->bind(1);
			program_->set_uniform("tex_normal", 1);
		}

		it = textures_.find("METAL_ROUGHNESS");
		if (it != textures_.end())
		{
			it->second->bind(2);
			program_->set_uniform("tex_orm", 2);
		}
	}
}	
