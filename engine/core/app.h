#pragma once
//------------------------------------------------------------------------------
/**
	Base class for applications, override this class
	and use it as an entry point for your lab.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <GL/glew.h>
namespace core
{
	class app
	{
	public:
		/// constructor
		app();
		/// destructor
		virtual ~app();

		/// open application
		virtual bool open();
		/// perform per-frame loop
		virtual void run();
		/// stop application
		virtual void close();
		/// kill application
		virtual void exit();

	private:
		bool is_open_;
	};
}