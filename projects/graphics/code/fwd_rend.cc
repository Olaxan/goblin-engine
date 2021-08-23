#include "fwd_rend.h"
#include <GL/glew.h>

namespace efiilj
{
	forward_renderer::forward_renderer(std::shared_ptr<camera_manager> camera_manager, std::shared_ptr<transform_manager> trf_mgr, const renderer_settings& set)
		: camera_mgr_(std::move(camera_manager)), _transforms(std::move(trf_mgr)), settings_(set), frame_index_(0), debug_(false), delta_time_() 
	{
		printf("Init forward renderer...\n");
		last_frame_ = frame_timer::now();
		printf("Forward renderer ready\n");
	}  

	void forward_renderer::reload_shaders() const
	{
		for (auto& node : nodes_)
			node->get_material()->get_program()->reload();
	}

	void forward_renderer::begin_frame()
	{
		delta_time_ = frame_timer::now() - last_frame_;
		last_frame_ = frame_timer::now();

		on_begin_frame();
	}

	void forward_renderer::render() const
	{
		for (auto& node : nodes_)
		{
			node->bind();
			node->draw();
			node->unbind();
		}
	}

	void forward_renderer::end_frame()
	{
		frame_index_++;	

		on_end_frame();
	}
}
