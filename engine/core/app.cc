//------------------------------------------------------------------------------
// app.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "app.h"

namespace core
{

app::app() :
	is_open_(false)
{
	// empty
}

app::~app()
= default;

bool app::open()
{
	assert(!this->is_open_);
	this->is_open_ = true;
	return true;
}

void app::run()
{
	// override in subclass
}

void app::close()
{
	assert(this->is_open_);
	this->is_open_ = false;
}

void app::exit()
{
	
}

} // namespace Application