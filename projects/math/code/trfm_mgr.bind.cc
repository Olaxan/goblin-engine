#include "trfm_mgr.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

namespace efiilj
{
	PYBIND11_EMBEDDED_MODULE(transform, m)
	{
		m.doc() = "Manipulate transform instances";

		py::class_<transform_manager>(m, "Transform")
			.def("register", &transform_manager::register_entity,
					"Register a new transform instance",
					py::arg("eid"))
			.def("unregister", &transform_manager::unregister_entity,
					"Unregister a transform instance",
					py::arg("eid"));
	};

	/*PYBIND11_MODULE(transform, m)
	{
		m.doc() = "Manipulate transform instances";

		m.def("register", &transform_manager::register_entity, 
				"Register a new transform instance", py::arg("eid"));

		m.def("unregister", &transform_manager::unregister_entity,
				"Unregister a transform instance", py::arg("eid"));

		m.def("set_position", &transform_manager::set_position,
				"Set the world position of an instance",
				py::arg("idx"), py::arg("position"));

		m.def("add_position", &transform_manager::add_position,
				"Add to the world position of an instance",
				py::arg("idx"), py::arg("delta"));

		m.def("get_position", &transform_manager::get_position,
				"Get the world position of an instance",
				py::arg("idx"));

		m.def("set_rotation", 
				py::overload_cast<transform_id, const vector3&>(&transform_manager::set_rotation),
				"Set the rotation of an instance in Euler angles",
				py::arg("idx"), py::arg("xyz"));

		m.def("set_rotation",
				py::overload_cast<transform_id, const quaternion&>(&transform_manager::set_rotation),
				"Set the rotation of an instance as a quaternion",
				py::arg("idx"), py::arg("q"));

		m.def("get_rotation", &transform_manager::get_rotation,
				"Get the rotation of an instance in Euler angles",
				py::arg("idx"));

		m.def("set_parent", &transform_manager::set_parent,
				"Set the parent of an instance",
				py::arg("idx"), py::arg("parent_id"));

		m.def("get_parent", &transform_manager::get_parent,
				"Get the parent of an instance",
				py::arg("idx"));
	}*/
}
