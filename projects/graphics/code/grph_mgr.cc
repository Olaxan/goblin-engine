#include "grph_mgr.h"

namespace efiilj 
{
	graphics_manager::graphics_manager(std::shared_ptr<transform_manager> trf_mgr)
		: _transforms(std::move(trf_mgr))
	{}

	graphics_manager::~graphics_manager()
	{
		printf("Graphics manager exit\n");
	}

	graphics_id graphics_manager::register_entity(entity_id eid)
	{
		graphics_id new_id = _instances.size();
		_instances.emplace_back(new_id);
		_instance_mapping.emplace(eid, new_id);

		_data.model_uri.emplace_back();
		_data.geometry_shader_uri.emplace_back();
		_data.lighting_shader_uri.emplace_back();
		_data.transform.emplace_back(-1);
		_data.loaded.emplace_back(false);

		return new_id;
	}

	bool graphics_manager::unregister_entity(graphics_id idx)
	{
		return false;
	}

	const fs::path& graphics_manager::get_model_uri(graphics_id idx)
	{
		return _data.model_uri[idx];
	}

	void graphics_manager::set_model_uri(graphics_id idx, const fs::path& uri)
	{
		_data.model_uri[idx] = uri;
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
