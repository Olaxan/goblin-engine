#pragma once

#include "rend_set.h"
#include "cam_mgr.h"
#include "node.h"
#include "scene.h"

#include <memory>
#include <chrono>

namespace efiilj
{
	class forward_renderer
	{
	protected:

		std::vector<std::shared_ptr<graphics_node>> nodes_;
		std::shared_ptr<camera_manager> camera_mgr_;
		const renderer_settings& settings_;

		bool debug_;

		unsigned frame_index_;

	private:

		typedef std::chrono::duration<float> duration;
		typedef std::chrono::high_resolution_clock frame_timer; 
		typedef std::chrono::time_point<frame_timer> frame_timer_point;

		frame_timer_point last_frame_;
		duration delta_time_;

	public:

		forward_renderer(std::shared_ptr<camera_manager> camera_manager, const renderer_settings& set);
		~forward_renderer() = default;

		virtual void add_node(std::shared_ptr<graphics_node> node)
		{
			this->nodes_.push_back(node);
		}

		virtual void add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes)
		{
			for (auto& node : nodes)
				add_node(node);
		}

		virtual void add_scene(const std::shared_ptr<scene> scene)
		{
			for (auto& node : scene->nodes)
				add_node(node);
		}

		virtual void reload_shaders() const;

		virtual void render() const;

		void begin_frame();
		void end_frame();

		virtual void on_begin_frame() {}
		virtual void on_end_frame() {}

		unsigned get_frame_index() const { return frame_index_; }
		float get_delta_time() const { return delta_time_.count(); }

		void set_debug(bool enabled) { debug_ = enabled; printf("Debug mode %i\n", debug_); }
		void toggle_debug() { set_debug(!debug_); }
	};
}
