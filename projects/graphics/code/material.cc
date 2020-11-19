#include "material.h"
#include <GL/glew.h>

namespace efiilj
{

	material_base::material_base(std::shared_ptr<shader_program> program)
		: program_(std::move(program)), double_sided(false), wireframe(false), color(vector4(1.0f, 1.0f, 1.0f, 1.0f))
	{ }

	gltf_pbr_base::gltf_pbr_base(std::shared_ptr<shader_program> program)
			: material_base(program), 
			emissive_factor(vector3(0, 0, 0)),
			metallic_factor(1.0f),
			roughness_factor(1.0f),
			alpha_cutoff(0.5f)
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

		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

		program_->set_uniform("base_color_factor", color);
		
	}

	void gltf_pbr_base::apply()
	{
		material_base::apply();

		program_->set_uniform("emissive_factor", emissive_factor);
		program_->set_uniform("metallic_factor", metallic_factor);
		program_->set_uniform("roughness_factor", roughness_factor);
		program_->set_uniform("alpha_cutoff", alpha_cutoff);

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

		it = textures_.find("EMISSIVE");
		if (it != textures_.end())
		{
			it->second->bind(3);
			program_->set_uniform("tex_emissive", 3);
		}
	}
}	
