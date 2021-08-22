#include "trfm_mgr.h"

namespace efiilj
{
	transform_manager::transform_manager()
	{}

	transform_manager::~transform_manager()
	{}

	transform_id transform_manager::register_entity(entity_id id)
	{
		transform_id trf_id = _data.model.size();
		_instances[id] = trf_id;

		_data.model.emplace_back(matrix4());
		_data.inverse.emplace_back(matrix4());
		_data.rotation.emplace_back(quaternion());
		_data.inverse_dirty.emplace_back(false);
		_data.model_dirty.emplace_back(false);

		return trf_id;
	}

	bool transform_manager::unregister_entity(transform_id idx)
	{
		return false;
	}
}
