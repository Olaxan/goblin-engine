#include "mgr_host.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

namespace efiilj
{
	PYBIND11_EMBEDDED_MODULE(managers, m)
	{

	};
}
