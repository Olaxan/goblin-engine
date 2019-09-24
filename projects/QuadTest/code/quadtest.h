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
	class QuadTest : public Core::App
	{
	private:

		Display::Window* window;
		std::chrono::time_point<std::chrono::steady_clock> t_start, t_now;
		float time;

	public:

		QuadTest();
		~QuadTest();

		bool Open();
		void Run();
	};
}