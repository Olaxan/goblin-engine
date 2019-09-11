//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"

int
main(int argc, const char** argv)
{
	Example::QuadTest app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
	
}