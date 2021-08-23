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
			std::vector<float> fov;
			std::vector<float> near;
			std::vector<float> far;

			std::vector<matrix4> perspective;
			std::vector<matrix4> p_inverse;
			std::vector<matrix4> view;

			std::vector<transform_id> transform;
		} _data;

		void setup_ubo();

		void update_perspective(camera_id idx);
		void push_perspective();

		void update_view(camera_id idx);
		void push_view();

	public:

		camera_manager(std::shared_ptr<transform_manager> trf_mgr);
		~camera_manager() = default;

		camera_id register_entity(entity_id eid) override;
		bool unregister_entity(camera_id idx) override;

		camera_id get_camera() const;
		bool set_camera(camera_id active);

		float get_width(camera_id idx) const;
		void set_width(camera_id idx, float width);

		float get_height(camera_id idx) const;
		void set_height(camera_id idx, float height);

		float get_fov(camera_id idx) const;
		void set_fov(camera_id idx, float fov);

		void set_size(camera_id idx, float width, float height);

		const matrix4& get_perspective(camera_id idx) const;

		const matrix4& get_view(camera_id idx) const;
		transform_id get_transform(camera_id idx) const;
		void set_transform(camera_id idx, transform_id trf);
	};
}
