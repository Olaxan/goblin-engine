#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"

#include "mesh_res.h"
#include "tex_res.h"

#include <chrono>

namespace Example
{
class QuadTest : public Core::App
{
public:
	/// constructor
	QuadTest();
	/// destructor
	~QuadTest();

	/// open app
	bool Open();
	/// run app
	void Run();
private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	efiilj::MeshResource mesh;
	efiilj::TextureResource texture;
	efiilj::Vertex* vertices;
	efiilj::Vertex newVert[36];
	Display::Window* window;
	std::chrono::time_point<std::chrono::steady_clock> t_start, t_now;
	float time;
};
} // namespace Example