//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"
#include "camera.h"
#include "loader.h"

#include <cstring>
#include <iostream>


using namespace Display;

namespace efiilj
{

	QuadTest::QuadTest() : window(nullptr), time(0), mouse_x(0), mouse_y(0), mouse_down_x(0), mouse_down_y(0), is_dragging_mouse(false) { }

	QuadTest::~QuadTest() = default;

	bool QuadTest::Open()
	{
		App::Open();

		t_start = std::chrono::high_resolution_clock::now();

		this->window = new Display::Window(1000, 1000);

		if (this->window->Open())
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

	void QuadTest::Run()
	{

		float fov = nvgDegToRad(75);

		std::string fs = shader_resource::load_shader("./res/shaders/vertex.shader");
		std::string vs = shader_resource::load_shader("./res/shaders/fragment.shader");

		object_loader foxLoader = object_loader("./res/meshes/fox.obj");
		object_loader rockLoader = object_loader("./res/meshes/rock.obj");

		if (!foxLoader.is_valid() || !rockLoader.is_valid())
		{
			std::cout << "\nFailed to load OBJ file - program will terminate.\n";
			return;
		}

		std::cout << "Loaded " << foxLoader.vertex_count() << " vertices, " << foxLoader.index_count() << " indices\n";
		std::cout << "Loaded " << rockLoader.vertex_count() << " vertices, " << rockLoader.index_count() << " indices\n";

		mesh_resource foxModel = foxLoader.get_resource();
		mesh_resource rockModel = rockLoader.get_resource();

		std::shared_ptr<shader_resource> shaderPtr = std::make_shared<shader_resource>(fs, vs);

		std::shared_ptr<mesh_resource> foxMeshPtr = std::make_shared<mesh_resource>(foxModel);
		std::shared_ptr<mesh_resource> rockMeshPtr = std::make_shared<mesh_resource>(rockModel);

		std::shared_ptr<texture_resource> foxTexturePtr = std::make_shared<texture_resource>("./res/textures/fox_base.png", true);
		std::shared_ptr<texture_resource> rockTexturePtr = std::make_shared<texture_resource>("./res/textures/rock_base.png", true);

		std::shared_ptr<transform_model> foxTransPtr = std::make_shared<transform_model>(Vector3(0, 0.5f, -1), Vector3(0), Vector3(0.1f, 0.1f, 0.1f));
		std::shared_ptr<transform_model> rockTransPtr = std::make_shared<transform_model>(Vector3(0, 0, -1), Vector3(1.6), Vector3(0.005f, 0.005f, 0.005f));

		std::shared_ptr<camera_model> cameraPtr = std::make_shared<camera_model>(fov, 1.0f, 0.1f, 100.0f,
			transform_model(Vector3(0, 2, 2), Vector3(0, 0, 0), Vector3(1, 1, 1)), Vector3(0, 1, 0));

		graphics_node foxNode(foxMeshPtr, foxTexturePtr, shaderPtr, foxTransPtr, cameraPtr);
		graphics_node rockNode(rockMeshPtr, rockTexturePtr, shaderPtr, rockTransPtr, cameraPtr);

		window->SetKeyPressFunction([&](int key, int scancode, int action, int mod)
			{
				if (action == 0)
					return;

				switch (key)
				{
				case GLFW_KEY_W:
					foxTransPtr->position(Vector3(0, 0, -0.05f), true);
					break;
				case GLFW_KEY_S:
					foxTransPtr->position(Vector3(0, 0, 0.05f), true);
					break;
				case GLFW_KEY_A:
					foxTransPtr->position(Vector3(-0.05f, 0, 0), true);
					break;
				case GLFW_KEY_D:
					foxTransPtr->position(Vector3(0.05f, 0, 0), true);
					break;
				case GLFW_KEY_LEFT_SHIFT:
					foxTransPtr->position(Vector3(0, -0.05f, 0), true);
					break;
				case GLFW_KEY_SPACE:
					foxTransPtr->position(Vector3(0, 0.05f, 0), true);
					break;

				default:
					window->Close();
					break;
				}
			});

		window->SetMouseMoveFunction([&](float x, float y)
			{
				mouse_x = x / 1000.0f - 0.5f;
				mouse_y = y / 1000.0f - 0.5f;

				cameraPtr->transform().rotation(Vector3(-0.5f - mouse_y, mouse_x - 3.1415f / 2, 0));

				//std::cout << "                             \rX: " << mouse_x << ", Y: " << mouse_y;

				if (is_dragging_mouse)
				{
					rockTransPtr->rotation(Vector3(mouse_y - mouse_down_y, mouse_x - mouse_down_x, 0) * 5, false);
				}
			});

		window->SetMousePressFunction([&](int button, int action, int mods) 
			{
				if (action == 1)
				{
					mouse_down_x = mouse_x;
					mouse_down_y = mouse_y;
					is_dragging_mouse = true;
				}
				else
				{
					is_dragging_mouse = false;
				}
			});

		while (this->window->IsOpen())
		{

			t_now = std::chrono::high_resolution_clock::now();
			time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();

			foxNode.draw();
			rockNode.draw();

			//cameraPtr->Transform().Rotation(Vector3(-0.5f, time, 0));

			this->window->SwapBuffers();
		}
	}
}