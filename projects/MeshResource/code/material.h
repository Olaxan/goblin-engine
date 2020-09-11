#pragma once

#include "tex_res.h"
#include "program.h"

#include <memory>
#include <vector>

namespace efiilj 
{
	class material_base
	{
	private:
		std::vector<std::shared_ptr<texture_resource>> textures_;
		std::shared_ptr<shader_program> program_;

	public: 
		material_base(std::shared_ptr<shader_program> program);
		material_base(std::shared_ptr<shader_program> program, std::vector<std::shared_ptr<texture_resource>> textures);

		void add_texture(std::shared_ptr<texture_resource> texture)
		{
			textures_.push_back(std::move(texture));
		}

		const shader_program& program() const { return *this->program_; }
		void program(std::shared_ptr<shader_program>& program) { this->program_ = std::move(program); }

		virtual void apply() = 0;
	};

	class material_blinnphong : material_base
	{
	public:
		material_blinnphong(std::shared_ptr<shader_program> program);
		material_blinnphong(std::shared_ptr<shader_program> program, std::vector<std::shared_ptr<texture_resource>> textures);
	};
}
