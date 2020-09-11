#pragma once

#include "tex_res.h"

#include <memory>

namespace efiilj 
{
	class material
	{
	private:
		std::shared_ptr<texture_resource> base_;
		std::shared_ptr<texture_resource> normal_;

	public: 
		material(std::shared_ptr<texture_resource> base, std::shared_ptr<texture_resource> normal);
	};
}
