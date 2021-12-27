#include "lght_mgr.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace efiilj
{
	PYBIND11_MODULE(lights, m)
	{
		m.doc() = "Add, remove, or manipulate light entities";
		/*
		m.def("register", &light_manager::register_entity, 
				"Register a new light entity", py::arg("eid"));
		m.def("unregister", &light_manager::unregister_entity,
				"Unregister a light entity", py::arg("eid"));*/
	}
}
