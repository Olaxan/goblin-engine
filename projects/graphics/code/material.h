#pragma once

#include "tex_res.h"
#include "program.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace efiilj 
{
	class material_base
	{
	protected:
		std::unordered_map<std::string, std::shared_ptr<texture_resource>> textures_;
		std::shared_ptr<shader_program> program_;

	public:

		bool double_sided;
		bool wireframe;
		vector4 color;

		material_base(std::shared_ptr<shader_program> program);

		void add_texture(std::string type, std::shared_ptr<texture_resource> texture)
		{
			textures_[type] = std::move(texture);
		}

		std::shared_ptr<shader_program> get_program() const { return this->program_; }
		void set_program(std::shared_ptr<shader_program> program) { this->program_ = std::move(program); }

		virtual void apply();
		virtual void draw_material_gui();
	};

	class gltf_pbr_base : public material_base
	{
	public:
		gltf_pbr_base(std::shared_ptr<shader_program> program);

		vector3 emissive_factor;
		float metallic_factor;
		float roughness_factor;
		float alpha_cutoff;

		void apply() override;
		void draw_material_gui() override;
	};
}
