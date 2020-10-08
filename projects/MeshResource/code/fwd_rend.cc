#include "fwd_rend.h"

namespace efiilj
{
	forward_renderer::forward_renderer(std::shared_ptr<camera_manager> camera_manager, const renderer_settings& set)
		: camera_mgr_(camera_manager), settings_(set) {}  

	void forward_renderer::add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes)
	{
		for (auto& node : nodes)
			this->nodes_.push_back(node);
	}

	void forward_renderer::reload_shaders() const
	{
		for (auto& node : nodes_)
			node->material().program().reload();
	}

	void forward_renderer::render() const
	{
		for (auto& node : nodes_)
			node->draw();
	}
}
