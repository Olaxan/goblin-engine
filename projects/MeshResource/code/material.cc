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
			// TEMP
			program_->set_uniform("u_light.color", vector3(1.0f, 1.0f, 1.0f));
			program_->set_uniform("u_light.intensity", vector3(1.0f, 1.0f, 1.0f));
			program_->set_uniform("u_light.position", vector3(2, 2, 2));
			program_->set_uniform("u_ambient_color", vector3(0.025f, 0, 0.025f));
			program_->set_uniform("u_ambient_strength", 1.0f);
			program_->set_uniform("u_specular_strength", 1.0f);
			program_->set_uniform("u_shininess", 128);

			program_->use();

			auto it = textures_.find("BASE");
			if (it != textures_.end())
			{
				it->second->bind(0);
				program_->set_uniform("u_base", 0);
			}

			it = textures_.find("NORMAL");
			if (it != textures_.end())
			{
				it->second->bind(1);
				program_->set_uniform("u_normal", 1);
			}
		}	
	}
}	
