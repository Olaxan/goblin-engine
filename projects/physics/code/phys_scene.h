#include <memory>
#include <vector>

#include "phys_mesh.h"
#include "phys_data.h"

namespace efiilj
{
	struct physics_scene
	{
		std::vector<std::shared_ptr<physics_node>> nodes;;
		std::vector<std::shared_ptr<physics_mesh>> meshes;

		std::string name;
	};
}
