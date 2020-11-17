#pragma once

#include "node.h"
#include "mesh_res.h"
#include "material.h"

namespace efiilj
{
	struct scene
	{
		std::vector<std::shared_ptr<graphics_node>> nodes;
		std::vector<std::shared_ptr<mesh_resource>> meshes;
		std::vector<std::shared_ptr<gltf_pbr_base>> materials;
	};
}
