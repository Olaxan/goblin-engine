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

	float time = 0;
	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	efiilj::MeshResource mesh;
	efiilj::TextureResource texture;
	efiilj::Vertex* vertices;
	efiilj::Vertex newVert[36];
	Display::Window* window;
};
} // namespace Example