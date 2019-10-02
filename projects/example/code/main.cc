//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"

int
main(int argc, const char** argv)
{
	example::example_app app;
	if (app.open())
	{
		app.run();
		app.close();
	}
	app.exit();
	
}