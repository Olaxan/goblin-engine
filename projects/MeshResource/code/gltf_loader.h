#pragma once

#include "tiny_gltf.h"
#include "mesh_res.h"
#include "node.h"

#include <string>

namespace efiilj 
{
	class gltf_model_loader
	{
	private:
		mesh_resource build_mesh(tinygltf::Model&, tinygltf::Mesh&);
		mesh_resource build_consolidated_mesh(tinygltf::Model&);
		void parse_node(tinygltf::Model&, tinygltf::Node&);
		unsigned get_meshes(tinygltf::Model&);
		
		mesh_resource mesh;
		std::vector<graphics_node> nodes_;

	public:
		gltf_model_loader();
		gltf_model_loader(std::string, bool=false);

		gltf_model_loader(gltf_model_loader&)
			= default;

		gltf_model_loader(gltf_model_loader&&)
			= default;
	
		static size_t type_component_count(std::string& type);
		static size_t component_type_size(int type);

		bool load_from_file(tinygltf::Model&, std::string, bool);

		mesh_resource get_resource()
		{
			return mesh;
		}
	
		~gltf_model_loader();
	};
}
