#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"

#include "cam_mgr.h"
#include "node.h"

#include <memory>

namespace efiilj
{
	class application final : public core::app
	{
	private:

		Display::Window* window_;
		double mouse_x_, mouse_y_, mouse_norm_x_, mouse_norm_y_, mouse_down_x_, mouse_down_y_;
		float time_;
		bool is_dragging_mouse_, is_mouse_captured_, is_software_renderer_;

		std::shared_ptr<camera_manager> cam_mgr_ptr;
		std::shared_ptr<graphics_node> selected_node_;
		
	public:

		application();
		~application();

		bool open() override;
		void run() override;

	};
}
