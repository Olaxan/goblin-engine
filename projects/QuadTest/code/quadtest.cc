//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"

#include "quadtest.h"
#include "loader.h"
#include "light.h"
#include "node.h"
#include "swrast.h"
#include "bufrend.h"
#include "gltf_loader.h"
#include "color.h"
#include "program.h"
#include "def_rend.h"
#include "fwd_rend.h"
#include "cam_mgr.h"

#include <chrono>
#include <iostream>
#include <set>

#define flase false
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1200
#define CAMERA_SPEED 0.5f

namespace efiilj
{

	quad_test::quad_test()
	: window_(nullptr), time_(0), mouse_x_(0), mouse_y_(0), mouse_down_x_(0), mouse_down_y_(0), is_dragging_mouse_(false), is_mouse_captured_(true), is_software_renderer_(flase) { }

	quad_test::~quad_test() = default;

	bool quad_test::open()
	{
		app::open();

		this->window_ = new Display::Window(WINDOW_WIDTH, WINDOW_HEIGHT);

		if (this->window_->Open())
		{

			//enable face culling
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			//lock mouse to window
			glfwSetInputMode(window_->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// set clear color
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			return true;
		}
		return false;
	}

	void quad_test::run()
	{

		auto g_vs = shader_resource(GL_VERTEX_SHADER, "../res/shaders/dvs_geometry.glsl");
		auto g_fs = shader_resource(GL_FRAGMENT_SHADER, "../res/shaders/dfs_geometry.glsl");
		auto g_prog_ptr = std::make_shared<shader_program>(g_vs, g_fs);

		auto l_vs = shader_resource(GL_VERTEX_SHADER, "../res/shaders/dvs_lighting.glsl");
		auto l_fs = shader_resource(GL_FRAGMENT_SHADER, "../res/shaders/dfs_lighting.glsl");
		auto l_prog_ptr = std::make_shared<shader_program>(l_vs, l_fs);

		auto h_vs = shader_resource(GL_VERTEX_SHADER, "../res/shaders/vs_gltf.glsl");
		auto h_fs = shader_resource(GL_FRAGMENT_SHADER, "../res/shaders/fs_gltf.glsl");
		auto h_prog_ptr = std::make_shared<shader_program>(h_vs, h_fs);

		auto cam_mgr_ptr = std::make_shared<camera_manager>(WINDOW_WIDTH, WINDOW_HEIGHT);
		
		renderer_settings set;

		set.width = WINDOW_WIDTH;
		set.height = WINDOW_HEIGHT;

		deferred_renderer def_renderer(cam_mgr_ptr, set, g_prog_ptr, l_prog_ptr);
		forward_renderer fwd_renderer(cam_mgr_ptr, set);
		
		auto sponza_trans_ptr = std::make_shared<transform_model>(vector3(0, 0, 0), vector3(0), vector3(0.05f, 0.05f, 0.05f));
		auto helmet_trans_ptr = std::make_shared<transform_model>(vector3(0, 0, 0), vector3(0), vector3(5.0f, 5.0f, 5.0f));
		
		gltf_model_loader gltf_sponza("../res/gltf/Sponza/Sponza.gltf", g_prog_ptr, sponza_trans_ptr);
		gltf_model_loader gltf_helmet("../res/gltf/FlightHelmet/FlightHelmet.gltf", h_prog_ptr, helmet_trans_ptr);

		object_loader light_sphere("../res/volumes/v_pointlight2.obj");

		def_renderer.add_nodes(gltf_sponza.get_nodes());
		fwd_renderer.add_nodes(gltf_helmet.get_nodes());

		std::vector<light_source> lights;

		auto sun_ptr = std::make_shared<light_source>();
		sun_ptr->base.ambient_intensity = 0.0001f;
		sun_ptr->base.diffuse_intensity = 0.01f;
		sun_ptr->type = light_type::directional;
		sun_ptr->update_falloff();

		def_renderer.add_light(sun_ptr);

		auto l_red_ptr = std::make_shared<light_source>();
		l_red_ptr->base.color = vector3(1.0f, 0.0f, 0.0f);
		l_red_ptr->base.ambient_intensity = 0.5f;
		l_red_ptr->base.diffuse_intensity = 1.0f;
		l_red_ptr->transform.add_position(vector4(0, 10, 0, 0));
		l_red_ptr->falloff.exponential = 0.1f;
		l_red_ptr->update_falloff();
		
		auto l_blue_ptr = std::make_shared<light_source>();
		l_blue_ptr->base.color = vector3(0.0f, 0.0f, 1.0f);
		l_blue_ptr->base.ambient_intensity = 0.5f;
		l_blue_ptr->base.diffuse_intensity = 1.0f;
		l_blue_ptr->transform.add_position(vector4(0, 10, 0, 0));
		l_blue_ptr->falloff.exponential = 0.1f;
		l_blue_ptr->update_falloff();

		def_renderer.add_light(l_red_ptr);
		def_renderer.add_light(l_blue_ptr);

		std::set<int> keys;
		
		window_->SetKeyPressFunction([&](const int key, int, const int action, int)
		{
			if (action == 1)
			{
				keys.emplace(key);

				if (key == GLFW_KEY_TAB)
				{
					if (is_mouse_captured_)
						window_->SetCursorMode(GLFW_CURSOR_NORMAL);
					else
						window_->SetCursorMode(GLFW_CURSOR_DISABLED);

					is_mouse_captured_ = !is_mouse_captured_;
				}
				else if (key == GLFW_KEY_LEFT_CONTROL)
				{
					is_software_renderer_ = !is_software_renderer_;
				}
				else if (key == GLFW_KEY_R)
				{
					def_renderer.reload_shaders();
					fwd_renderer.reload_shaders();
				}
			}
			else if (action == 0)
			{
				const auto it = keys.find(key);
				if (it != keys.end())
					keys.erase(it);
			}
		});

		window_->SetMouseMoveFunction([&](const float x, const float y)
		{
			mouse_x_ = x / 1000.0f - 0.5f;
			mouse_y_ = y / 1000.0f - 0.5f;
		});

		window_->SetMousePressFunction([&](const int button, const int action, int mods) 
		{
			if (action == 1)
			{
				mouse_down_x_ = mouse_x_;
				mouse_down_y_ = mouse_y_;
				is_dragging_mouse_ = true;
			}
			else
			{
				is_dragging_mouse_ = false;
			}
		});

		while (this->window_->IsOpen())
		{
			auto camera_ptr = cam_mgr_ptr->get_active_camera();
			auto camera_trans_ptr = camera_ptr->get_transform();

			if (is_mouse_captured_)
				camera_trans_ptr->set_rotation(vector4(-mouse_y_, 0, mouse_x_, 1));
			else if (is_dragging_mouse_)
				helmet_trans_ptr->add_rotation(vector4(mouse_y_ - mouse_down_y_, mouse_x_ - mouse_down_x_, 0, 1) * 0.5f);
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->window_->Update();
	
			if (keys.find(GLFW_KEY_W) != keys.end())
				camera_trans_ptr->add_position(camera_trans_ptr->forward() * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_S) != keys.end())
				camera_trans_ptr->add_position(camera_trans_ptr->forward() * -CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_A) != keys.end())
				camera_trans_ptr->add_position(camera_trans_ptr->left() * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_D) != keys.end())
				camera_trans_ptr->add_position(camera_trans_ptr->left() * -CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_SPACE) != keys.end())
				camera_trans_ptr->add_position(camera_trans_ptr->up() * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_LEFT_SHIFT) != keys.end())
				camera_trans_ptr->add_position(camera_trans_ptr->up() * -CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_ESCAPE) != keys.end())
				window_->Close();
			
			cam_mgr_ptr->update_camera();

			l_red_ptr->transform.set_position(vector4(sinf(def_renderer.get_frame_index() / 100.0f) * 25, 10.0f, 20.0f, 1.0));
			l_blue_ptr->transform.set_position(vector4(cosf(def_renderer.get_frame_index() / 100.0f) * 25, 10.0f, -20.0f, 1.0));

			def_renderer.begin_frame();
			def_renderer.render();
			def_renderer.end_frame();

			fwd_renderer.begin_frame();
			fwd_renderer.render();
			fwd_renderer.end_frame();

			this->window_->SwapBuffers();
			
		}
	}
}

#undef flase
