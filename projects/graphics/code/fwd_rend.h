#pragma once

#include "manager.h"
#include "ifmgr.h"

#include "rend_set.h"
#include "mesh_res.h"
#include "material.h"

#include "lght_mgr.h"
#include "cam_mgr.h"

#include <memory>
#include <chrono>

namespace efiilj
{
	typedef int render_id;

	class forward_renderer : public manager<render_id>, public registrable
	{
	protected:

		const renderer_settings& settings_;

		unsigned frame_index_;

		struct RenderData
		{
			std::vector<std::shared_ptr<mesh_resource>> mesh;
			std::vector<std::shared_ptr<material_base>> material;
			std::vector<transform_id> transform;
		} _data;

		std::shared_ptr<camera_manager> _cameras;
		std::shared_ptr<transform_manager> _transforms;
		std::shared_ptr<light_manager> _lights;

	private:

		typedef std::chrono::duration<float> duration;
		typedef std::chrono::high_resolution_clock frame_timer; 
		typedef std::chrono::time_point<frame_timer> frame_timer_point;

		frame_timer_point last_frame_;
		duration delta_time_;

	public:

		forward_renderer(
				std::shared_ptr<camera_manager> camera_manager, 
				std::shared_ptr<transform_manager> trf_mgr, 
				std::shared_ptr<light_manager> light_mgr,
				const renderer_settings& set);
		~forward_renderer() = default;

		render_id register_entity(entity_id eid) override;
		bool  unregister_entity(render_id idx) override;

		void draw_gui() override;
		void draw_gui(render_id idx) override;

		void set_mesh(render_id idx, std::shared_ptr<mesh_resource> mesh);

		void set_material(render_id idx, std::shared_ptr<material_base> mat);

		void set_transform(render_id idx, transform_id transform);
		
		void render(render_id idx) const;
		virtual void render_frame() const;

		void begin_frame();
		void end_frame();

		virtual void on_begin_frame() {}
		virtual void on_end_frame() {}

		unsigned get_frame_index() const { return frame_index_; }
		float get_delta_time() const { return delta_time_.count(); }
	};
}
