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

#include <iostream>
#include <set>

#define flase false

namespace efiilj
{

	quad_test::quad_test()
	: window_(nullptr), frame_(0), mouse_x_(0), mouse_y_(0), mouse_down_x_(0), mouse_down_y_(0), is_dragging_mouse_(false), is_mouse_captured_(true), is_software_renderer_(flase) { }

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

		float fov = 1.30899694; // 75 degrees

		std::string vs_source = shader_resource::load_shader("./res/shaders/gltf.vertex");
		std::string fs_source = shader_resource::load_shader("./res/shaders/gltf.fragment");

		auto vs = shader_resource(GL_VERTEX_SHADER, vs_source);
		auto fs = shader_resource(GL_FRAGMENT_SHADER, fs_source);
		auto prog_ptr = std::make_shared<shader_program>(vs, fs);

		auto trans_ptr = std::make_shared<transform_model>(vector3(0, 0, 0), vector3(0), vector3(0.5f, 0.5f, 0.5f));
		auto camera_trans_ptr = std::make_shared<transform_model>(vector3(0, 0, 0), vector3(0), vector3(1, 1, 1));
		auto camera_ptr = std::make_shared<camera_model>(fov, 1.0f, 0.1f, 100.0f, camera_trans_ptr, vector3(0, 1, 0));
		
		gltf_model_loader gltf_loader("./res/gltf/FlightHelmet/glTF/FlightHelmet.gltf", prog_ptr, trans_ptr);

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
					camera_trans_ptr->rotation = vector4(-mouse_y_, mouse_x_, 0, 1);
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
		
		while (this->window_->IsOpen())
		{

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

		//	shader_ptr->use();
		//	shader_ptr->set_uniform("u_camera_position", camera_trans_ptr->position);
		//	shader_ptr->set_uniform("u_light.color", p_light.rgb);
		//	shader_ptr->set_uniform("u_light.intensity", p_light.intensity);
		//	shader_ptr->set_uniform("u_light.position", p_light.position); // should not be vec4!
		//	shader_ptr->set_uniform("u_ambient_color", vector3(0.025f, 0, 0.025f));
		//	shader_ptr->set_uniform("u_ambient_strength", 1.0f);
		//	shader_ptr->set_uniform("u_specular_strength", 0.5f);
		//	shader_ptr->set_uniform("u_shininess", 32);
		//	shader_ptr->drop();
			
			for (auto& node : gltf_loader.get_nodes())
			{
				node.draw(camera_ptr, frame_);
			}

			this->window_->SwapBuffers();

			frame_++;
		}
	}
}

#undef flase
