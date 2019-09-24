//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"
#include <cstring>
#include <iostream>


using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
QuadTest::QuadTest()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
QuadTest::~QuadTest()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
QuadTest::Open()
{
	App::Open();
	this->window = new Display::Window(720, 720);
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
	});

	t_start = std::chrono::high_resolution_clock::now();

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

//------------------------------------------------------------------------------
/**
*/
void
QuadTest::Run()
{

	float fov = nvgDegToRad(75);

	std::string fs = efiilj::ShaderResource::LoadShader("./res/shaders/vertex.shader");
	std::string vs = efiilj::ShaderResource::LoadShader("./res/shaders/fragment.shader");

	efiilj::ShaderResource shader = efiilj::ShaderResource(fs.c_str(), vs.c_str());
	efiilj::TextureResource texture = efiilj::TextureResource("./res/textures/test.png", true);
	efiilj::MeshResource mesh = efiilj::MeshResource::Cube(1);

	efiilj::GraphicsNode node(mesh, texture, shader);

	while (this->window->IsOpen())
	{

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		efiilj::Matrix4 model = efiilj::Matrix4::getScale(1, 1, 1);
		efiilj::Matrix4 view = efiilj::Matrix4::getLookat(efiilj::Vector3(2 * cosf(time), 1, 2 * sinf(time)), efiilj::Vector3(0.0f, 0.0f, 0.0f), efiilj::Vector3(0.0f, 1.0f, 0.0f));
		efiilj::Matrix4 perspective = efiilj::Matrix4::getPerspective(fov, 1.0f, 0.1f, 100.0f);

		efiilj::Matrix4 mvp = (perspective * view * model);

		shader.Use();
		shader.SetUniformMatrix4fv("mvp", mvp);
		shader.SetUniform1i("u_sampler", 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();

		node.Draw();

		this->window->SwapBuffers();
	}
}

} // namespace Example