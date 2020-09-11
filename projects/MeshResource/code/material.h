#pragma once

#include "tex_res.h"
#include "program.h"

#include <memory>

namespace efiilj 
{
	class material_base
	{
	private:
		std::shared_ptr<texture_resource> base_;
		std::shared_ptr<texture_resource> normal_;
		std::shared_ptr<shader_program> program_;

	public: 
		material_base(std::shared_ptr<texture_resource> base, std::shared_ptr<texture_resource> normal);

		const shader_program& program() const { return *this->program_; }
		void program(std::shared_ptr<shader_program>& program) { this->program_ = std::move(program); }

		virtual void apply() = 0;
	};
}
