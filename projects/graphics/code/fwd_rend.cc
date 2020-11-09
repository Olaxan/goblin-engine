#include "fwd_rend.h"
#include <GL/glew.h>

namespace efiilj
{
	forward_renderer::forward_renderer(std::shared_ptr<camera_manager> camera_manager, const renderer_settings& set)
		: camera_mgr_(camera_manager), settings_(set), frame_index_(0), debug_(false), delta_time_() 
	{
		last_frame_ = frame_timer::now();
	}  

	void forward_renderer::reload_shaders() const
	{
		for (auto& node : nodes_)
			node->material().get_program()->reload();
	}

	void forward_renderer::begin_frame()
	{
		delta_time_ = frame_timer::now() - last_frame_;
		last_frame_ = frame_timer::now();

		on_begin_frame();
	}

	void forward_renderer::render() const
	{
		if (debug_)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			for (auto& node : nodes_)
			{
				node->bind();
				node->draw();
				node->unbind();
			}

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			// boo
			for (auto& node : nodes_)
			{
				node->bind();
				node->draw();
				node->unbind();
			}
		}
	}

	void forward_renderer::end_frame()
	{
		frame_index_++;	

		on_end_frame();
	}
}
