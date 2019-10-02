#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"

#include "node.h"

#include <chrono>

namespace efiilj
{
	class quad_test final : public core::app
	{
	public:

		quad_test();
		~quad_test();

		bool open() override;
		void run() override;

	private:

		Display::Window* window_;
		std::chrono::time_point<std::chrono::steady_clock> t_start_, t_now_;
		float time_, mouse_x_, mouse_y_, mouse_down_x_, mouse_down_y_;
		bool is_dragging_mouse_;
	};
}