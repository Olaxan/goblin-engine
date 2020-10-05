#pragma once

#include "program.h"

namespace efiilj
{
	class deferred_renderer
	{
	private:
		int width, height;
		unsigned gbo, rbo, pos, norm, cspec;
		unsigned attachments[3];

		std::shared_ptr<shader_program> geometry_;
		std::shared_ptr<shader_program> lighting_;

		void gen_buffer(unsigned* handle, unsigned type);
	public:
		deferred_renderer(int width, int height, std::shared_ptr<shader_program> geometry, std::shared_ptr<shader_program> lighting);
		~deferred_renderer() = default;
	};
}
