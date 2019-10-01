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

	QuadTest::QuadTest() : window(0), time(0), mouse_x(0), mouse_y(0), mouse_down_x(0), mouse_down_y(0), is_dragging_mouse(false) { }

	QuadTest::~QuadTest() { }

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

		std::string fs = ShaderResource::LoadShader("./res/shaders/vertex.shader");
		std::string vs = ShaderResource::LoadShader("./res/shaders/fragment.shader");

		ObjectLoader foxLoader = ObjectLoader("./res/meshes/fox.obj");
		ObjectLoader rockLoader = ObjectLoader("./res/meshes/rock.obj");

		if (!foxLoader.isValid() || !rockLoader.isValid())
		{
			std::cout << "\nFailed to load OBJ file - program will terminate.\n";
			return;
		}

		std::cout << "Loaded " << foxLoader.vertexCount() << " vertices, " << foxLoader.indexCount() << " indices\n";
		std::cout << "Loaded " << rockLoader.vertexCount() << " vertices, " << rockLoader.indexCount() << " indices\n";

		MeshResource foxModel = foxLoader.GetResource();
		MeshResource rockModel = rockLoader.GetResource();

		std::shared_ptr<ShaderResource> shaderPtr = std::make_shared<ShaderResource>(fs, vs);

		std::shared_ptr<MeshResource> foxMeshPtr = std::make_shared<MeshResource>(foxModel);
		std::shared_ptr<MeshResource> rockMeshPtr = std::make_shared<MeshResource>(rockModel);

		std::shared_ptr<TextureResource> foxTexturePtr = std::make_shared<TextureResource>("./res/textures/fox_base.png", true);
		std::shared_ptr<TextureResource> rockTexturePtr = std::make_shared<TextureResource>("./res/textures/rock_base.png", true);

		std::shared_ptr<TransformModel> foxTransPtr = std::make_shared<TransformModel>(Vector3(0, 1.5f, -1), Vector3(0), Vector3(0.1f, 0.1f, 0.1f));
		std::shared_ptr<TransformModel> rockTransPtr = std::make_shared<TransformModel>(Vector3(0, 0, -1), Vector3(0), Vector3(0.01f, 0.01f, 0.01f));

		std::shared_ptr<CameraModel> cameraPtr = std::make_shared<CameraModel>(fov, 1.0f, 0.1f, 100.0f,
			TransformModel(Vector3(0, 2, 2), Vector3(0, 0, 0), Vector3(1, 1, 1)), Vector3(0, 1, 0));

		GraphicsNode foxNode(foxMeshPtr, foxTexturePtr, shaderPtr, foxTransPtr, cameraPtr);
		GraphicsNode rockNode(rockMeshPtr, rockTexturePtr, shaderPtr, rockTransPtr, cameraPtr);

		window->SetKeyPressFunction([&](int32 key, int32 scancode, int32 action, int32 mod)
			{
				if (action == 0)
					return;

				switch (key)
				{
				case GLFW_KEY_W:
					foxTransPtr->Position(Vector3(0, 0, -0.05f), true);
					break;
				case GLFW_KEY_S:
					foxTransPtr->Position(Vector3(0, 0, 0.05f), true);
					break;
				case GLFW_KEY_A:
					foxTransPtr->Position(Vector3(-0.05f, 0, 0), true);
					break;
				case GLFW_KEY_D:
					foxTransPtr->Position(Vector3(0.05f, 0, 0), true);
					break;
				case GLFW_KEY_LEFT_SHIFT:
					foxTransPtr->Position(Vector3(0, -0.05f, 0), true);
					break;
				case GLFW_KEY_SPACE:
					foxTransPtr->Position(Vector3(0, 0.05f, 0), true);
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

				cameraPtr->Transform().Rotation(Vector3(-0.5f - mouse_y, mouse_x - 3.1415f / 2, 0));

				//std::cout << "                             \rX: " << mouse_x << ", Y: " << mouse_y;

				if (is_dragging_mouse)
				{
					rockTransPtr->Rotation(Vector3(mouse_y - mouse_down_y, mouse_x - mouse_down_x, 0) * 5, false);
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

			foxNode.Draw();
			rockNode.Draw();

			//cameraPtr->Transform().Rotation(Vector3(-0.5f, time, 0));

			this->window->SwapBuffers();
		}
	}
}