#include "meta_mgr.h"

#include "imgui.h"

namespace efiilj
{

	meta_manager::meta_manager()
	{
		printf("Init metadata ...\n");
		_name = "Metadata";
	}

	meta_manager::~meta_manager()
	{
		printf("Metadata manager exit\n");
	}

	void meta_manager::extend_defaults(meta_id new_id)
	{
		_data.name.emplace_back();
		_data.description.emplace_back();
	}

	void meta_manager::on_editor_gui() {}

	void meta_manager::on_editor_gui(meta_id idx)
	{
		if (is_valid(idx))
		{
			ImGui::Text("%s", _data.name[idx].c_str());
			ImGui::TextWrapped("%s", _data.description[idx].c_str());
		}
	}

	void meta_manager::on_register(std::shared_ptr<manager_host> host) {}
}
