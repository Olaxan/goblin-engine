#include "cam_mgr.h"

#include <GL/glew.h>

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
		_instance_mapping[eid] = new_id;
	}

	void camera_manager::setup_ubo()
	{
		glGenBuffers(1, &_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(matrix4), nullptr, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo, 0, 2 * sizeof(matrix4));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void camera_manager::push_perspective()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrix4), &_data.perspective[_current]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void camera_manager::push_view()
	{	
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(matrix4), sizeof(matrix4), &_data.view[_current]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	bool camera_manager::set_camera(unsigned active)
	{
		if (active < cameras_.size())
		{
			active_camera_ = active;

			glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_, 0, 2 * sizeof(matrix4));
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrix4), &cameras_[active_camera_]->get_perspective());
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			return true;
		}

		return false;
	}
}
