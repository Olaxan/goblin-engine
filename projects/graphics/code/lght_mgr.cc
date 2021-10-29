#include "lght_mgr.h"
#include "mgr_host.h"
#include "imgui.h"

namespace efiilj
{
	light_manager::light_manager()
	{
		printf("Init light manager...\n");
		_name = "Light";
	}

	light_manager::~light_manager()
	{
		printf("Light manager exit\n");
	}

	void light_manager::on_activate(light_id idx)
	{
		entity_id eid = get_entity(idx);
		transform_id trf_id = _transforms->get_component(eid);
		_data.transform[idx] = trf_id;
	}

	void light_manager::on_editor_gui()
	{ }

	void light_manager::on_editor_gui(light_id idx)
	{
		if (!is_valid(idx))
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "No light selected");
			return;
		}

		if (ImGui::TreeNode("Light data"))
		{

			ImGui::Text("Transform %d", _data.transform[idx]);

			ImGui::Text("Type");

			if (ImGui::RadioButton("Directional", _data.type[idx] == light_type::directional)) 
			{ _data.type[idx] = light_type::directional; }
			ImGui::SameLine(); 
			if (ImGui::RadioButton("Point", _data.type[idx] == light_type::pointlight)) 
			{ _data.type[idx] = light_type::pointlight; }
			ImGui::SameLine(); 
			if (ImGui::RadioButton("Spot", _data.type[idx] == light_type::spotlight)) 
			{ _data.type[idx] = light_type::spotlight; }

			if (ImGui::TreeNode("Light Base"))
			{
				ImGui::ColorPicker3("Color", &_data.base[idx].color.x);
				ImGui::DragFloat("Ambient intensity", &_data.base[idx].ambient_intensity, 0.01f);
				ImGui::DragFloat("Diffuse intensity", &_data.base[idx].diffuse_intensity, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Attenuation"))
			{
				if (ImGui::DragFloat("Constant", &_data.attenuation[idx].constant, 0.01f, 0, FLT_MAX)
				|| ImGui::DragFloat("Linear", &_data.attenuation[idx].linear, 0.01f, 0, FLT_MAX)
				|| ImGui::DragFloat("Exponential", &_data.attenuation[idx].exponential, 0.01f, 0, FLT_MAX))
					update_falloff(idx);

				ImGui::TreePop();
			}

			if (_data.type[idx] == light_type::spotlight)
			{
				if (ImGui::TreeNode("Cutoff"))
				{
					ImGui::SliderFloat("Inner", &_data.cutoff[idx].inner_angle, 0.1f, 3.1415f);
					ImGui::SliderFloat("Outer", &_data.cutoff[idx].outer_angle, 0.1f, 3.1415f);
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}
	
	void light_manager::on_register(std::shared_ptr<manager_host> host)
	{
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');

		add_data({
				&_data.attenuation,
				&_data.base,
				&_data.cutoff,
				&_data.transform,
				&_data.type});
	}

	void light_manager::on_validate(entity_id eid)
	{
		const auto& lights = get_components(eid);

		for (auto idx : lights)
		{
			_data.transform[idx] = _transforms->get_component(eid);
			if (_data.transform[idx] == -1)
				set_error(idx);
		}
	}

	void light_manager::update_falloff(light_id idx)
	{

		const light_base& base = _data.base[idx];
		const attenuation_data& falloff = _data.attenuation[idx];
		const transform_id& trf_id = _data.transform[idx];

		float max_channel = fmax(fmax(base.color.x, base.color.y), base.color.z);

		float ret = (-falloff.linear +
			sqrtf(falloff.linear * falloff.linear -
				4 * falloff.exponential * (
					falloff.constant - 256 *
					max_channel * base.diffuse_intensity
					)
			)
			) / (2 * falloff.exponential);

		_transforms->set_scale(trf_id, ret * 2.0f);
	}

	const light_type& light_manager::get_type(light_id idx) const
	{
		return _data.type[idx];
	}

	void light_manager::set_type(light_id idx, const light_type& type)
	{
		_data.type[idx] = type;
	}

	const light_base& light_manager::get_base(light_id idx) const
	{
		return _data.base[idx];
	}

	void light_manager::set_base(light_id idx, const light_base& base)
	{
		_data.base[idx] = base;
	}

	void light_manager::set_base(light_id idx, const vector3& color, const float& ambient, const float& diffuse)
	{
		set_base(idx, {color, ambient, diffuse});
	}

	const attenuation_data& light_manager::get_attenuation(light_id idx) const
	{
		return _data.attenuation[idx];
	}

	void light_manager::set_attenuation(light_id idx, const attenuation_data& att)
	{
		_data.attenuation[idx] = att;
	}

	void light_manager::set_attenuation(light_id idx, const float& constant, const float& linear, const float& exponential)
	{
		set_attenuation(idx, {constant, linear, exponential});
	}

	const cutoff_data& light_manager::get_cutoff(light_id idx) const
	{
		return _data.cutoff[idx];
	}

	void light_manager::set_cutoff(light_id idx, const cutoff_data& cutoff)
	{
		_data.cutoff[idx] = cutoff;
	}

	void light_manager::set_cutoff(light_id idx, const float& inner, const float& outer)
	{
		set_cutoff(idx, {inner, outer});
	}

	const transform_id& light_manager::get_transform(light_id idx) const
	{
		return _data.transform[idx];
	}

	void light_manager::set_transform(light_id idx, const transform_id& transform)
	{
		_data.transform[idx] = transform;
	}
}
