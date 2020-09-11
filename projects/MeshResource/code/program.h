#pragma once

#include "shader_res.h""

namespace efiilj
{
	class program
	{
	private:
		shader_resource vs_;
		shader_resource fs_;

	public:

		program(shader_resource vs, shader_resource fs);

		
	};
}
