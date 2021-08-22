#pragma once

#include "camera.h"
#include "entity.h"
#include "trfm_mgr.h"

#include <vector>
#include <memory>

namespace efiilj
{
	typedef int camera_id;

	class camera_manager : public manager<camera_id>
	{
	private:

		std::shared_ptr<transform_manager> _transforms;

		unsigned _ubo;
		camera_id _current;

		struct 
		{
			std::vector<float> width;
			std::vector<float> height;

			std::vector<matrix4> perspective;
			std::vector<matrix4> p_inverse;
			std::vector<matrix4> view;

			std::vector<transform_id> transform;
		} _data;

		void setup_ubo();

	public:

		camera_manager(std::shared_ptr<transform_manager> trf_mgr);
		~camera_manager() = default;

		camera_id register_entity(entity_id eid);
		bool unregister_entity(camera_id idx);

		bool set_camera(camera_id active);

		void push_perspective();
		void push_view();
	};
}
