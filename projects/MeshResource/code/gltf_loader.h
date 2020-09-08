#pragma once

#include "tiny_gltf.h"

#include <string>

namespace efiilj 
{
	class gltf_model_loader
	{
	private:

	public:

		gltf_model_loader();
		gltf_model_loader(std::string, bool=false);

		gltf_model_loader(gltf_model_loader&)
			= default;

		gltf_model_loader(gltf_model_loader&&)
			= default;
		
		bool load_from_file(tinygltf::Model&, std::string, bool);

		void build_mesh(tinygltf::Model&, tinygltf::Mesh&);
		void get_nodes(tinygltf::Model&, tinygltf::Node&);
		unsigned get_meshes(tinygltf::Model&);

		~gltf_model_loader();
	};
}
