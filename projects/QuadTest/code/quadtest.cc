//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"
#include "camera.h"
#include "loader.h"
#include "light.h"
#include "node.h"
#include "swrast.h"
#include "bufrend.h"
#include "gltf_loader.h"
#include "color.h"
#include "program.h"
#include "def_rend.h"

#include <chrono>
#include <iostream>
#include <set>

#define flase false

namespace efiilj
{

	quad_test::quad_test()
	: window_(nullptr), time_(0), mouse_x_(0), mouse_y_(0), mouse_down_x_(0), mouse_down_y_(0), is_dragging_mouse_(false), is_mouse_captured_(true), is_software_renderer_(flase) { }

	quad_test::~quad_test() = default;

	bool quad_test::open()
	{
		app::open();

		this->window_ = new Display::Window(1024, 1024);

		if (this->window_->Open())
		{

			//enable face culling
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			//lock mouse to window
			glfwSetInputMode(window_->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// set clear color to purple
			glClearColor(0.01f, 0.0f, 0.01f, 0.5f);

			return true;
		}
		return false;
	}

	void quad_test::run()
	{

		typedef std::chrono::duration<float> duration;
		typedef std::chrono::high_resolution_clock frame_timer; 
		typedef std::chrono::time_point<frame_timer> frame_timer_point; 

		auto g_vs = shader_resource(GL_VERTEX_SHADER, "./res/shaders/def_geometry.vertex");
		auto g_fs = shader_resource(GL_FRAGMENT_SHADER, "./res/shaders/def_geometry.fragment");
		auto g_prog_ptr = std::make_shared<shader_program>(g_vs, g_fs);

		auto l_vs = shader_resource(GL_VERTEX_SHADER, "./res/shaders/def_lighting.vertex");
		auto l_fs = shader_resource(GL_FRAGMENT_SHADER, "./res/shaders/def_lighting.fragment");
		auto l_prog_ptr = std::make_shared<shader_program>(l_vs, l_fs);
		
		renderer_settings set;
		deferred_renderer renderer(set, g_prog_ptr, l_prog_ptr);

		auto camera_ptr = renderer.get_active_camera();
		auto camera_trans_ptr = camera_ptr->get_transform();
		auto trans_ptr = std::make_shared<transform_model>(vector3(0, 0, 0), vector3(0), vector3(0.5f, 0.5f, 0.5f));
		
		//gltf_model_loader gltf_loader("./res/gltf/Sponza/Sponza.gltf", prog_ptr, trans_ptr);
		gltf_model_loader gltf_loader("./res/gltf/FlightHelmet/glTF/FlightHelmet.gltf", nullptr, trans_ptr);

		point_light p_light = point_light(vector3(0.5f, 0.5f, 0.5f), vector3(1.0f, 1.0f, 1.0f), vector3(2, 2, 2));
	
		

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
						renderer.reload_shaders();
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

				if (is_mouse_captured_)
					camera_trans_ptr->rotation = vector4(-mouse_y_, 0, mouse_x_, 1);
				else if (is_dragging_mouse_)
					trans_ptr->rotation += vector4(mouse_y_ - mouse_down_y_, mouse_x_ - mouse_down_x_, 0, 1) * 0.5f;
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

		frame_timer_point begin_frame;
		duration dt;

		begin_frame = frame_timer::now();

		while (this->window_->IsOpen())
		{
			dt = frame_timer::now() - begin_frame;
			time_ += dt.count();
			begin_frame = frame_timer::now();

			// Add something to wrap time around to avoid imprecision.
			// Not a priority.

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window_->Update();

			if (keys.find(GLFW_KEY_W) != keys.end())
				camera_trans_ptr->position+= camera_trans_ptr->forward() * 0.01f;
			
			if (keys.find(GLFW_KEY_S) != keys.end())
				camera_trans_ptr->position -= camera_trans_ptr->forward() * 0.01f;
			
			if (keys.find(GLFW_KEY_A) != keys.end())
				camera_trans_ptr->position += camera_trans_ptr->left() * 0.01f;
			
			if (keys.find(GLFW_KEY_D) != keys.end())
				camera_trans_ptr->position -= camera_trans_ptr->left() * 0.01f;
			
			if (keys.find(GLFW_KEY_SPACE) != keys.end())
				camera_trans_ptr->position += camera_trans_ptr->up() * 0.01f;
			
			if (keys.find(GLFW_KEY_LEFT_SHIFT) != keys.end())
				camera_trans_ptr->position -= camera_trans_ptr->up() * 0.01f;
			
			if (keys.find(GLFW_KEY_ESCAPE) != keys.end())
				window_->Close();

			

			p_light.position = vector3(sinf(time_ / 120.0f) * 50.0f, 50.0f, cosf(time_ / 120.0f) * 50.0f); 
	
			prog_ptr->use();	
			prog_ptr->set_uniform("camera_position", camera_trans_ptr->position);	
			prog_ptr->set_uniform("light.color", p_light.rgb);
			prog_ptr->set_uniform("light.intensity", p_light.intensity);
			prog_ptr->set_uniform("light.position", p_light.position);
			prog_ptr->set_uniform("ambient_color", vector3(0.025f, 0, 0.025f));
			prog_ptr->set_uniform("ambient_strength", 1.0f);
			prog_ptr->set_uniform("specular_strength", 1.0f);

			prog_ptr->set_uniform("time", time_);
			prog_ptr->set_uniform("deltatime", dt.count());

			for (auto& node : gltf_loader.get_nodes())
			{
				node.draw();
			}

			this->window_->SwapBuffers();
			
		}
	}
}

#undef flase
