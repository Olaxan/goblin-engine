#include "material.h"

namespace efiilj
{

	material_base::material_base(std::shared_ptr<shader_program> program)
		: program_(std::move(program))
	{ }

	gltf_pbr_base::gltf_pbr_base(std::shared_ptr<shader_program> program)
		: material_base{ program } 
	{ }

	void gltf_pbr_base::apply()
	{
		if (program_ != nullptr)
		{
			program_->set_uniform("shininess", 128);

			program_->use();

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
}	
