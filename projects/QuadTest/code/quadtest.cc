//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"
#include "camera.h"
#include "loader.h"

#include <iostream>


using namespace Display;

namespace efiilj
{

	quad_test::quad_test() : window_(nullptr), time_(0), mouse_x_(0), mouse_y_(0), mouse_down_x_(0), mouse_down_y_(0), is_dragging_mouse_(false) { }

	quad_test::~quad_test() = default;

	bool quad_test::open()
	{
		app::open();

		t_start_ = std::chrono::high_resolution_clock::now();

		this->window_ = new Display::Window(1000, 1000);

		if (this->window_->Open())
		{

			//enable face culling
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			// set clear color to purple
			glClearColor(0.025f, 0.0f, 0.025f, 1.0f);

			return true;
		}
		return false;
	}

	void quad_test::run()
	{

		float fov = nvgDegToRad(75);

		std::string fs = shader_resource::load_shader("./res/shaders/vertex.shader");
		std::string vs = shader_resource::load_shader("./res/shaders/fragment.shader");

		object_loader fox_loader = object_loader("./res/meshes/fox.obj");
		object_loader rock_loader = object_loader("./res/meshes/rock.obj");

		if (!fox_loader.is_valid() || !rock_loader.is_valid())
		{
			std::cout << "\nFailed to load OBJ file - program will terminate.\n";
			return;
		}

		std::cout << "Loaded " << fox_loader.vertex_count() << " vertices, " << fox_loader.index_count() << " indices\n";
		std::cout << "Loaded " << rock_loader.vertex_count() << " vertices, " << rock_loader.index_count() << " indices\n";

		mesh_resource fox_model = fox_loader.get_resource();
		mesh_resource rock_model = rock_loader.get_resource();

		auto shader_ptr = std::make_shared<shader_resource>(fs, vs);

		auto fox_mesh_ptr = std::make_shared<mesh_resource>(fox_model);
		auto rock_mesh_ptr = std::make_shared<mesh_resource>(rock_model);

		auto fox_texture_ptr = std::make_shared<texture_resource>("./res/textures/fox_base.png", true);
		auto rock_texture_ptr = std::make_shared<texture_resource>("./res/textures/rock_base.png", true);

		auto fox_trans_ptr = std::make_shared<transform_model>(Vector3(0, 0.5f, -1), Vector3(0), Vector3(0.1f, 0.1f, 0.1f));
		auto rock_trans_ptr = std::make_shared<transform_model>(Vector3(0, 0, -1), Vector3(1.6), Vector3(0.005f, 0.005f, 0.005f));

		auto camera_ptr = std::make_shared<camera_model>(fov, 1.0f, 0.1f, 100.0f,
			transform_model(Vector3(0, 2, 2), Vector3(0, 0, 0), Vector3(1, 1, 1)), Vector3(0, 1, 0));

		graphics_node fox_node(fox_mesh_ptr, fox_texture_ptr, shader_ptr, fox_trans_ptr, camera_ptr);
		graphics_node rock_node(rock_mesh_ptr, rock_texture_ptr, shader_ptr, rock_trans_ptr, camera_ptr);

		window_->SetKeyPressFunction([&](const int key, int, const int action, int)
			{
				if (action == 0)
					return;

				switch (key)
				{
				case GLFW_KEY_W:
					fox_trans_ptr->position(Vector3(0, 0, -0.05f), true);
					break;
				case GLFW_KEY_S:
					fox_trans_ptr->position(Vector3(0, 0, 0.05f), true);
					break;
				case GLFW_KEY_A:
					fox_trans_ptr->position(Vector3(-0.05f, 0, 0), true);
					break;
				case GLFW_KEY_D:
					fox_trans_ptr->position(Vector3(0.05f, 0, 0), true);
					break;
				case GLFW_KEY_LEFT_SHIFT:
					fox_trans_ptr->position(Vector3(0, -0.05f, 0), true);
					break;
				case GLFW_KEY_SPACE:
					fox_trans_ptr->position(Vector3(0, 0.05f, 0), true);
					break;

				default:
					window_->Close();
					break;
				}
			});

		window_->SetMouseMoveFunction([&](float x, float y)
			{
				mouse_x_ = x / 1000.0f - 0.5f;
				mouse_y_ = y / 1000.0f - 0.5f;

				camera_ptr->transform().rotation(Vector3(-0.5f - mouse_y_, mouse_x_ - 3.1415f / 2, 0));

				if (is_dragging_mouse_)
				{
					rock_trans_ptr->rotation(Vector3(mouse_y_ - mouse_down_y_, mouse_x_ - mouse_down_x_, 0) * 5, false);
				}
			});

		window_->SetMousePressFunction([&](int button, const int action, int mods) 
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

			t_now_ = std::chrono::high_resolution_clock::now();
			time_ = std::chrono::duration_cast<std::chrono::duration<float>>(t_now_ - t_start_).count();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window_->Update();

			fox_node.draw();
			rock_node.draw();

			//cameraPtr->Transform().Rotation(Vector3(-0.5f, time, 0));

			this->window_->SwapBuffers();
		}
	}
}