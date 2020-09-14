#include "material.h"

namespace efiilj
{

	material_base::material_base(std::shared_ptr<shader_program> program)
		: program_(std::move(program))
	{
		
	}

	gltf_pbr_base::gltf_pbr_base(std::shared_ptr<shader_program> program)
		: material_base{ program } { }

	void gltf_pbr_base::apply()
	{
			
	}
}	
