#include "material.h"

namespace efiilj
{

	material_base::material_base(std::shared_ptr<shader_program> program)
		: program_(std::move(program))
	{
		
	}

	material_base::material_base(std::shared_ptr<shader_program> program, std::vector<std::shared_ptr<texture_resource>> textures) 
		: program_(std::move(program))
       	{ 
		for (auto tex_ptr : textures)
		{
			textures_.push_back(std::move(tex_ptr));
		}
	}

	material_blinnphong::material_blinnphong(std::shared_ptr<shader_program> program)
		: material_base{ program } { }

	material_blinnphong::material_blinnphong(std::shared_ptr<shader_program> program, std::vector<std::shared_ptr<texture_resource>> textures)
		: material_base{ program, textures } { }

	void material_blinnphong::apply()
	{
	
	}
}	
