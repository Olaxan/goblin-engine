#pragma once

#include "camera.h"

#include <vector>
#include <memory>

namespace efiilj
{
	class camera_manager
	{
	private:

		unsigned ubo_, active_camera_;
		
		unsigned width_, height_;

		vector3 up_;

		std::vector<std::shared_ptr<camera_model>> cameras_;

		void setup_ubo();

	public:
		camera_manager(unsigned width, unsigned height);
		~camera_manager() = default;

		bool set_camera(unsigned active);

		void update_camera();

		std::shared_ptr<camera_model> get_default_camera() const { return cameras_[0]; }
		std::shared_ptr<camera_model> get_active_camera() const { return cameras_[active_camera_]; }

		vector3 get_active_position() const { return get_active_camera()->get_transform()->get_position(); };
		//vector3 get_active_rotation() const { return get_active_camera()->get_transform()->get_rotation(); };
	};
}
