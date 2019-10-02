#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
namespace example
{
class example_app final : public core::app
{
public:
	/// constructor
	example_app();
	/// destructor
	~example_app();

	/// open app
	bool open() override;
	/// run app
	void run() override;
private:

	unsigned int program_{};
	unsigned int vertex_shader_{};
	unsigned int pixel_shader_{};
	unsigned int triangle_{};
	Display::Window* window_{};
};
} // namespace Example