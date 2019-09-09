#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"

#include "..\..\projects\MeshResource\code\mesh.h"

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
	Display::Window* window;
};
} // namespace Example