#include "material.h"

namespace efiilj
{
	material::material(std::shared_ptr<texture_resource> base, std::shared_ptr<texture_resource> normal) 
		: base_(std::move(base)), normal_(std::move(normal)) { }	
}
