#include "entity.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace efiilj
{
	PYBIND11_MODULE(entities, m)
	{
		m.doc() = "Create and remove game entities";

		m.def("create", &entity_manager::create, "Create a new game entity");
		m.def("destroy", &entity_manager::destroy, "Destroy a game entity");
	}
}
