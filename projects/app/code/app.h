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

#include "mtrl_srv.h"
#include "mesh_srv.h"
#include "tex_srv.h"

#include "cam_mgr.h"
#include "mesh_mgr.h"
#include "trfm_mgr.h"
#include "lght_mgr.h"
#include "grph_mgr.h"
#include "shdr_mgr.h"
#include "mtrl_mgr.h"
#include "def_rend.h"
#include "sim.h"

#include <memory>

namespace efiilj
{
	class application final : public core::app
	{
	private:

		Display::Window* window_;

		float time_;

		double mouse_x, mouse_y;
		double mouse_down_x, mouse_down_y;
		double mouse_norm_x, mouse_norm_y;

		bool is_dragging_mouse;
		bool is_mouse_captured;

		std::shared_ptr<entity_manager> entities;
		std::shared_ptr<manager_host> managers;

		// Servers hold resources not connected to any particular entity
		std::shared_ptr<shader_server> shaders;
		std::shared_ptr<texture_server> textures;
		std::shared_ptr<material_server> materials;
		std::shared_ptr<mesh_server> meshes;

		// Managers hold components that can be attached to entities
		std::shared_ptr<transform_manager> transforms;
		std::shared_ptr<camera_manager> cameras;
		std::shared_ptr<graphics_manager> graphics;
		std::shared_ptr<forward_renderer> rfwd;
		std::shared_ptr<deferred_renderer> rdef;
		std::shared_ptr<light_manager> lights;
		std::shared_ptr<material_manager> material_instances;
		std::shared_ptr<mesh_manager> mesh_instances;
		std::shared_ptr<simulator> sim;
		
	public:

		application();
		~application();

		bool open() override;
		void run() override;

	};
}
