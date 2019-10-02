//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"

int
main(int argc, const char** argv)
{
	efiilj::quad_test app;
	if (app.open())
	{
		app.run();
		app.close();
	}
	app.exit();
	
}