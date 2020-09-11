#include "material.h"

namespace efiilj
{
	material_base::material_base(std::shared_ptr<texture_resource> base, std::shared_ptr<texture_resource> normal) 
		: base_(std::move(base)), normal_(std::move(normal)) { }	
}
