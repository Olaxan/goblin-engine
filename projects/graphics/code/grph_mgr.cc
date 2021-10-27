#include "grph_mgr.h"
#include "mgr_host.h"

namespace fs = std::filesystem;

namespace efiilj 
{
	graphics_manager::graphics_manager()
	{
		printf("Init graphics...\n");
		_name = "Graphics instance";
	}

	graphics_manager::~graphics_manager()
	{
		printf("Graphics manager exit\n");
	}

	void graphics_manager::extend_defaults(graphics_id new_id)
	{
		_data.model_uri.emplace_back();
		_data.geometry_shader_uri.emplace_back();
		_data.lighting_shader_uri.emplace_back();
		_data.transform.emplace_back(-1);
		_data.loaded.emplace_back(false);
	}

	void graphics_manager::on_editor_gui()
	{}

	void graphics_manager::on_editor_gui(graphics_id idx)
	{}

	void graphics_manager::on_register(std::shared_ptr<manager_host> host)
	{}

	const fs::path& graphics_manager::get_model_uri(graphics_id idx)
	{
		return _data.model_uri[idx];
	}

	void graphics_manager::set_model_uri(graphics_id idx, const fs::path& uri)
	{
		_data.model_uri[idx] = uri;
	}

	const fs::path& graphics_manager::get_geometry_shader_uri(graphics_id idx)
	{
		return _data.geometry_shader_uri[idx];
	}

	void graphics_manager::set_geometry_shader_uri(graphics_id idx, const fs::path& uri)
	{
		_data.geometry_shader_uri[idx] = uri;
	}

	const fs::path& graphics_manager::get_lighting_shader_uri(graphics_id idx)
	{
		return _data.lighting_shader_uri[idx];
	}

	void graphics_manager::set_lighting_shader_uri(graphics_id idx, const fs::path& uri)
	{
		_data.lighting_shader_uri[idx] = uri;
	}

	const transform_id& graphics_manager::get_transform(graphics_id idx)
	{
		return _data.transform[idx];
	}

	void graphics_manager::set_transform(graphics_id idx, transform_id trf)
	{
		_data.transform[idx] = trf;
	}
}
