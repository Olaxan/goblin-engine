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
		void build_mesh(tinygltf::Model&, tinygltf::Mesh&);
		void parse_node(tinygltf::Model&, tinygltf::Node&);
		unsigned get_meshes(tinygltf::Model&);
		
		mesh_resource mesh;
		std::vector<graphics_node> nodes_;

	public:
		gltf_model_loader();
		gltf_model_loader(const std::string& path, bool is_binary=false);

		gltf_model_loader(gltf_model_loader&)
			= default;

		gltf_model_loader(gltf_model_loader&&)
			= default;
	
		static size_t type_component_count(const std::string& type);
		static size_t component_type_size(int type);
		static unsigned get_format(int components);
		static unsigned get_type(int bits);

		bool load_from_file(tinygltf::Model& model, const std::string& path, bool is_binary);

		void draw() const;

		mesh_resource get_resource()
		{
			return mesh;
		}
	
		~gltf_model_loader();
	};
}
