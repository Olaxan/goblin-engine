#pragma once

#include "rend_set.h"
#include "cam_mgr.h"
#include "node.h"

#include <memory>

namespace efiilj
{
	class forward_renderer
	{
	private:
		std::vector<std::shared_ptr<graphics_node>> nodes_;
		std::shared_ptr<camera_manager> camera_mgr_;
		const renderer_settings& settings_;
	public:
		forward_renderer(std::shared_ptr<camera_manager> camera_manager, const renderer_settings& set);
		~forward_renderer() = default;

		void add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes);
		void reload_shaders() const;
		void render() const;
	};
}
