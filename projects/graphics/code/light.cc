#include "light.h"

#include <imgui.h>

namespace efiilj
{

	void light_source::update_falloff()
	{
		float max_channel = fmax(fmax(base_.color.x, base_.color.y), base_.color.z);

		float ret = (-falloff_.linear +
			sqrtf(falloff_.linear * falloff_.linear -
				4 * falloff_.exponential * (
					falloff_.constant - 256 *
					max_channel * base_.diffuse_intensity
					)
			)
			) / (2 * falloff_.exponential);

		transform_->set_scale(ret * 2.0f);
	}

	void light_source::set_uniforms(std::shared_ptr<shader_program> program) const
	{
		program->set_uniform("source.type", static_cast<int>(type_));
		program->set_uniform("source.base.color", base_.color);
		program->set_uniform("source.base.ambient_intensity", base_.ambient_intensity);
		program->set_uniform("source.base.diffuse_intensity", base_.diffuse_intensity);
		program->set_uniform("source.position", transform_->get_position());
		program->set_uniform("source.direction", transform_->forward());
		program->set_uniform("source.falloff.constant", falloff_.constant);
		program->set_uniform("source.falloff.linear", falloff_.linear);
		program->set_uniform("source.falloff.exponential", falloff_.exponential);
		program->set_uniform("source.cutoff.inner", cutoff_.inner_angle);
		program->set_uniform("source.cutoff.outer", cutoff_.outer_angle);
	}

	void light_source::draw_light_gui()
	{
		transform_->draw_transform_gui();

		ImGui::Text("Type");
		if (ImGui::RadioButton("Directional", type_ == light_type::directional)) { type_ = light_type::directional; }
		ImGui::SameLine(); if (ImGui::RadioButton("Point", type_ == light_type::pointlight)) { type_ = light_type::pointlight; }
		ImGui::SameLine(); if (ImGui::RadioButton("Spot", type_ == light_type::spotlight)) { type_ = light_type::spotlight; }

		ImGui::Text("Light base");
		ImGui::ColorPicker3("Color", &base_.color.x);
		ImGui::DragFloat("Ambient intensity", &base_.ambient_intensity);
		ImGui::DragFloat("Diffuse intensity", &base_.diffuse_intensity);

		ImGui::Text("Attenuation");
		ImGui::DragFloat("Constant", &falloff_.constant, 0, FLT_MAX);
		ImGui::DragFloat("Linear", &falloff_.linear, 0, FLT_MAX);
		ImGui::DragFloat("Exponential", &falloff_.exponential, 0, FLT_MAX);

		if (type_ == light_type::spotlight)
		{
			ImGui::Text("Cutoff");
			ImGui::SliderFloat("Inner", &cutoff_.inner_angle, 0.1f, 3.1415f);
			ImGui::SliderFloat("Outer", &cutoff_.outer_angle, 0.1f, 3.1415f);
		}
	}
}
