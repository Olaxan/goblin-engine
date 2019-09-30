//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"
#include "camera.h"

#include <cstring>
#include <iostream>


using namespace Display;

namespace efiilj
{

	QuadTest::QuadTest()
	{
		// empty
	}

	QuadTest::~QuadTest()
	{
		// empty
	}

	bool QuadTest::Open()
	{
		App::Open();

		t_start = std::chrono::high_resolution_clock::now();

		this->window = new Display::Window(1000, 1000);

		window->SetMouseMoveFunction([this](float64, float64) 
		{

		});

		if (this->window->Open())
		{

			//enable face culling
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			// set clear color to black
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

		MeshResource cube = MeshResource::Cube(1);

		std::shared_ptr<MeshResource> meshPtr = std::make_shared<MeshResource>(cube);
		std::shared_ptr<TextureResource> texturePtr = std::make_shared<TextureResource>("./res/textures/test.png", true);
		std::shared_ptr<ShaderResource> shaderPtr = std::make_shared<ShaderResource>(fs, vs);
		std::shared_ptr<TransformModel> transPtr1 = std::make_shared<TransformModel>(Vector3(0, 0, 0));
		std::shared_ptr<TransformModel> transPtr2 = std::make_shared<TransformModel>(Vector3(0, 1, 0), Vector3(0, 0.5f, 0));
		std::shared_ptr<CameraModel> cameraPtr = std::make_shared<CameraModel>(fov, 1.0f, 0.1f, 100.0f,
			TransformModel(Vector3(0, 2, 2), Vector3(0, 0, 0), Vector3(1, 1, 1)), Vector3(0, 1, 0));

		GraphicsNode node1(meshPtr, texturePtr, shaderPtr, transPtr1, cameraPtr);
		GraphicsNode node2(meshPtr, texturePtr, shaderPtr, transPtr2, cameraPtr);

		window->SetKeyPressFunction([&](int32 key, int32 scancode, int32 action, int32 mod)
		{
			switch (key)
			{
			case GLFW_KEY_W:
				transPtr1->Position(Vector3(0, 0, -0.1f), true);
				break;
			case GLFW_KEY_S:
				transPtr1->Position(Vector3(0, 0, 0.1f), true);
				break;
			case GLFW_KEY_A:
				transPtr1->Position(Vector3(-0.1f, 0, 0), true);
				break;
			case GLFW_KEY_D:
				transPtr1->Position(Vector3(0.1f, 0, 0), true);
				break;

			default:
				window->Close();
				break;
			}
		});

		window->SetMouseMoveFunction([&](float x, float y)
		{
				float xx = x / 1000.0f - 0.5f;
				float yy = y / 1000.0f - 0.5f;

				cameraPtr->Transform().Rotation(Vector3(-0.5f + yy, xx - 3.1415f / 2, 0));

				std::cout << "\rX: " << xx << ", Y: " << yy;
		});

		while (this->window->IsOpen())
		{

			t_now = std::chrono::high_resolution_clock::now();
			time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();

			node1.Draw();
			node2.Draw();

			//cameraPtr->Transform().Rotation(Vector3(-0.5f, time, 0));

			this->window->SwapBuffers();
		}
	}
}