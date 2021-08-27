#pragma once

#include "entity.h"
#include "trfm_mgr.h"
#include "ifmgr.h"
#include "ray.h"

#include <vector>
#include <memory>

namespace efiilj
{
	typedef int camera_id;

	class camera_manager : public manager<camera_id>
	{
	private:

		unsigned _ubo;
		camera_id _current;

		struct CameraData
		{
			std::vector<float> width;
			std::vector<float> height;
			std::vector<float> fov;
			std::vector<float> near;
			std::vector<float> far;

			std::vector<matrix4> perspective;
			std::vector<matrix4> p_inverse;
			std::vector<matrix4> view;

			std::vector<transform_id> transform;
		} _data;

		std::shared_ptr<transform_manager> _transforms;

		void setup_ubo();

		void update_perspective(camera_id idx);
		void update_view(camera_id idx);

		void push_perspective();
		void push_view();

	public:

		camera_manager();
		~camera_manager() = default;

		void extend_defaults(camera_id eid) override;
		void draw_gui() override;
		void draw_gui(camera_id idx) override;
		
		void on_register(std::shared_ptr<manager_host> host) override;

		void update();

		ray get_ray_from_camera(camera_id idx, int mouse_x, int mouse_y) const;

		camera_id get_camera() const;
		bool set_camera(camera_id active);

		float get_width(camera_id idx) const;
		void set_width(camera_id idx, float width);

		float get_height(camera_id idx) const;
		void set_height(camera_id idx, float height);

		void set_size(camera_id idx, float width, float height);

		float get_fov(camera_id idx) const;
		void set_fov(camera_id idx, float fov);

		float get_near(camera_id idx) const;
		void set_near(camera_id idx, float near);

		float get_far(camera_id idx) const;
		void set_far(camera_id idx, float fov);

		const matrix4& get_perspective(camera_id idx) const;

		const matrix4& get_view(camera_id idx) const;
		transform_id get_transform(camera_id idx) const;
		void set_transform(camera_id idx, transform_id trf);

		vector3 get_position(camera_id idx = -1)
		{
			return _transforms->get_position(_data.transform[idx == -1 ? _current : idx]);
		}
	};
}
