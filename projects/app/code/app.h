#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"

#include "entity.h"
#include "mgr_host.h"
#include "cam_mgr.h"
#include "trfm_mgr.h"
#include "def_rend.h"
#include "lght_mgr.h"
#include "grph_mgr.h"
#include "sim.h"

#include <memory>

namespace efiilj
{
	class application final : public core::app
	{
	private:

		Display::Window* window_;

		double mouse_x, mouse_y;
		double mouse_norm_x, mouse_norm_y;
		double mouse_down_x, mouse_down_y;

		float time_;
		bool is_dragging_mouse;
		bool is_mouse_captured;

		std::shared_ptr<entity_manager> entities;
		std::shared_ptr<manager_host> managers;

		std::shared_ptr<transform_manager> transforms;
		std::shared_ptr<camera_manager> cameras;
		std::shared_ptr<graphics_manager> graphics;
		std::shared_ptr<forward_renderer> rfwd;
		std::shared_ptr<deferred_renderer> rdef;
		std::shared_ptr<light_manager> lights;
		std::shared_ptr<simulator> sim;
		
	public:

		application();
		~application();

		bool open() override;
		void run() override;

	};
}
