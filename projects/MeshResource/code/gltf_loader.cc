#include "gltf_loader.h"

#define TINYGLTF_IMPLEMENTATION

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "tiny_gltf.h"
#include "mesh_res.h"
#include "vertex.h"

#include <stdio.h>
#include <vector>
#include <GL/glew.h>

namespace efiilj
{
	gltf_model_loader::gltf_model_loader()
	{

	}

	gltf_model_loader::gltf_model_loader(std::string path, bool is_binary)
	{
		tinygltf::Model model;
		if (load_from_file(model, path, is_binary))
		{
			printf("GLTF file successfully loaded\n");
			get_meshes(model);
		}
		else
		{
			printf("Failed to load GLTF\n");
		}
	}

	bool gltf_model_loader::load_from_file(tinygltf::Model& model, std::string path, bool is_binary)
	{
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;
		bool ret;
		
		if (is_binary)
			ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
		else
			ret = loader.LoadASCIIFromFile(&model, &err, &warn, path); // for binary glTF(.glb)

		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty()) {
			printf("Err: %s\n", err.c_str());
		}

		return ret;	
	
	}

	void gltf_model_loader::build_mesh(tinygltf::Model& model, tinygltf::Mesh& mesh)
	{
		printf("Constructing, %lu buffer views\n", model.bufferViews.size());

		for (int i = 0; i < model.bufferViews.size(); i++)
		{
			tinygltf::BufferView view = model.bufferViews[i];
			tinygltf::Buffer& buf = model.buffers[view.buffer];

			std::cout << "buffer.data.size = " << buf.data.size()
              		<< ", bufferview.byteOffset = " << view.byteOffset
              		<< std::endl;	

			//mesh_resource mesh(); // Get vertex list and index list from GLTF?
		}	
	}

	void gltf_model_loader::get_nodes(tinygltf::Model& model, tinygltf::Node& node)
	{
		if (node.mesh >= 0 && node.mesh < model.meshes.size())
		{
			build_mesh(model, model.meshes[node.mesh]);				
		}
		for (size_t i = 0; i < node.children.size(); i++) 
		{
			assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
			get_nodes(model, model.nodes[node.children[i]]);
		}
	}

	unsigned gltf_model_loader::get_meshes(tinygltf::Model& model)
	{
		const tinygltf::Scene scene = model.scenes[model.defaultScene];

		printf("Parsing GLTF file, %lu nodes in default scene %d\n", scene.nodes.size(), model.defaultScene);

		for (int i = 0; i < scene.nodes.size(); i++)
		{
			tinygltf::Node& node = model.nodes[scene.nodes[i]];
			get_nodes(model, node);
		}

		return 0;
	}

	gltf_model_loader::~gltf_model_loader()
	{
		printf("GLTF loader exited");
	}
}

