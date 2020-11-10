#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"

namespace efiilj
{
	class application final : public core::app
	{
	private:

		Display::Window* window_;
		float time_, mouse_x_, mouse_y_, mouse_down_x_, mouse_down_y_;
		bool is_dragging_mouse_, is_mouse_captured_, is_software_renderer_;
		
	public:

		application();
		~application();

		bool open() override;
		void run() override;

	};
}
