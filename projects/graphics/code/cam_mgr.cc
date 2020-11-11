#include "cam_mgr.h"

#include <GL/glew.h>

#define DEFAULT_FOV 1.30899694f
#define DEFAULT_NEAR 0.1f
#define DEFAULT_FAR 100.0f


namespace efiilj
{
	camera_manager::camera_manager(unsigned width, unsigned height)
	: ubo_(0), active_camera_(0), width_(width), height_(height), up_(vector3(0, 1, 0))
	{
		auto trf_ptr = std::make_shared<transform_model>(vector3(0, 0, 0), vector3(0), vector3(1, 1, 1));
		auto cam = std::make_shared<camera_model>(DEFAULT_FOV, width, height, DEFAULT_NEAR, DEFAULT_FAR, trf_ptr, up_);

		cameras_.push_back(cam);
		
		setup_ubo();
	}

	void camera_manager::setup_ubo()
	{
		glGenBuffers(1, &ubo_);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(matrix4), nullptr, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_, 0, 2 * sizeof(matrix4));
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrix4), &cameras_[0]->get_perspective());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void camera_manager::update_camera()
	{	
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(matrix4), sizeof(matrix4), &cameras_[active_camera_]->get_view());
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
