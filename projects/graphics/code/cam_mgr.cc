#include "cam_mgr.h"

#include <GL/glew.h>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 1024
#define DEFAULT_FOV 1.30899694f
#define DEFAULT_NEAR 0.1f
#define DEFAULT_FAR 100.0f


namespace efiilj
{
	camera_manager::camera_manager(std::shared_ptr<transform_manager> trf_mgr)
	: _transforms(std::move(trf_mgr)), _ubo(0), _current(0) 
	{
		setup_ubo();
	}

	camera_id camera_manager::register_entity(entity_id eid)
	{
		camera_id new_id = _instances.size();
		_instances.emplace_back(new_id);
		_instance_mapping.emplace(eid, new_id);

		_data.width.emplace_back(DEFAULT_WIDTH);
		_data.height.emplace_back(DEFAULT_HEIGHT);
		_data.fov.emplace_back(DEFAULT_FOV);
		_data.near.emplace_back(DEFAULT_NEAR);
		_data.far.emplace_back(DEFAULT_FAR);
		_data.perspective.emplace_back(matrix4());
		_data.p_inverse.emplace_back(matrix4());
		_data.view.emplace_back(matrix4());
		_data.transform.emplace_back(-1);

		update_perspective(new_id);
		update_view(new_id);

		return new_id;
	}

	void camera_manager::setup_ubo()
	{
		glGenBuffers(1, &_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(matrix4), nullptr, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo, 0, 2 * sizeof(matrix4));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void camera_manager::update_perspective(camera_id idx)
	{
		float aspect = _data.width[idx] / _data.height[idx];
		_data.perspective[idx] = matrix4::get_perspective(_data.fov[idx], aspect, _data.near[idx], _data.far[idx]);
	}

	void camera_manager::push_perspective()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrix4), &_data.perspective[_current]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void camera_manager::update_view(camera_id idx)
	{
		vector3 pos = _transforms->get_position(_data.transform[idx]);
		vector3 fwd = _transforms->get_forward(_data.transform[idx]);
		vector3 up = _transforms->get_up(_data.transform[idx]);
		_data.view[idx] = matrix4::get_lookat(pos, fwd, up);
	}

	void camera_manager::push_view()
	{	
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(matrix4), sizeof(matrix4), &_data.view[_current]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	camera_id camera_manager::get_camera() const
	{
		return _current;
	}
	
	bool camera_manager::set_camera(camera_id active)
	{
		if (active < _instances.size())
		{
			_current = active;
			push_view();
			return true;
		}

		return false;
	}

	float camera_manager::get_width(camera_id idx) const
	{
		return _data.width[idx];
	}

	void camera_manager::set_width(camera_id idx, float width)
	{
		_data.width[idx] = width;
		push_view();
	}
}
