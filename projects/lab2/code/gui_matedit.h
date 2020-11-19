#pragma once

#include "imgui.h"
#include "node.h"

namespace efiilj
{
	void show_material_editor(std::shared_ptr<material_base> mat)
	{
		ImGui::Begin("Material");
		
		if (mat == nullptr)
		{
			ImGui::Text("No material selected");
		}
		else
		{

		}

		ImGui::End();
	}
};
