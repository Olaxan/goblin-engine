#include "gltf_loader.h"

#define TINYGLTF_IMPLEMENTATION

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "tiny_gltf.h"
#include "node.h"

#include <stdio.h>
#include <vector>
#include <GL/glew.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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

	size_t gltf_model_loader::type_component_count(std::string& type)
	{
		if (type.compare("SCALAR") == 0)
			return 1;
		if (type.compare("VEC2") == 0)
			return 2;
		if (type.compare("VEC3") == 0)
			return 3;
		if (type.compare("VEC4") == 0)
			return 4;
		if (type.compare("MAT2") == 0)
			return 4;
		if (type.compare("MAT3") == 0)
			return 9;
		if (type.compare("MAT4") == 0)
			return 16;

		return 0;
	}

	size_t gltf_model_loader::component_type_size(int type) 
	{
		switch (type) 
		{
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			case TINYGLTF_COMPONENT_TYPE_BYTE:
				return sizeof(char);
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			case TINYGLTF_COMPONENT_TYPE_SHORT:
				return sizeof(short);
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			case TINYGLTF_COMPONENT_TYPE_INT:
				return sizeof(int);
			case TINYGLTF_COMPONENT_TYPE_FLOAT:
				return sizeof(float);
			case TINYGLTF_COMPONENT_TYPE_DOUBLE:
				return sizeof(double);
			default:
				return 0;
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

	mesh_resource gltf_model_loader::build_mesh(tinygltf::Model& model, tinygltf::Mesh& mesh)
	{
		printf("Constructing mesh %s\n", mesh.name.c_str());
		
		tinygltf::Primitive prim = mesh.primitives[0];

		unsigned err;
		unsigned vbo, vao, ibo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		size_t vbo_size = 0;
		for (auto &attrib : prim.attributes)
		{
			tinygltf::Accessor accessor = model.accessors[attrib.second];
			tinygltf::BufferView view = model.bufferViews[accessor.bufferView];
			vbo_size += accessor.count * accessor.ByteStride(view); 
		}
		
		printf("Buffer size %lu bytes\n", vbo_size);
		glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_STATIC_DRAW);

		
		int vertex_count = -1;
		size_t block_offset = 0;
		for (auto &attrib : prim.attributes)
		{
			tinygltf::Accessor accessor = model.accessors[attrib.second];
			tinygltf::BufferView view = model.bufferViews[accessor.bufferView];
			tinygltf::Buffer buf = model.buffers[view.buffer];
			
			size_t stride = accessor.ByteStride(view);
			size_t block_size = stride * accessor.count;
			size_t offset = view.byteOffset + accessor.byteOffset;

			printf("Attribute: %s, accessor %d, stride %ld, view %d (%s)\n", 
					attrib.first.c_str(), attrib.second, stride, accessor.bufferView, view.name.c_str());

			glBufferSubData(GL_ARRAY_BUFFER, block_offset, block_size, &buf.data.at(0) + offset);
			
			err = glGetError();
			printf("Buffering %ld bytes (offset %ld) to VBO %u in VAO %u... ERR = %d\n", block_size, block_offset, vbo, vao, err);

			int vaa = -1;
			int size = (accessor.type == TINYGLTF_TYPE_SCALAR) ? 1 : accessor.type;
			if (attrib.first.compare("POSITION") == 0)
			{
				vaa = 0;
				vertex_count = accessor.count;
			}

			if (attrib.first.compare("TANGENT") == 0)
				vaa = 1;

			if (attrib.first.compare("NORMAL") == 0)
				vaa = 2;

			if (attrib.first.compare("TEXCOORD_0") == 0)
				vaa = 3;

			glEnableVertexAttribArray(vaa);
			glVertexAttribPointer(vaa, size, accessor.componentType, accessor.normalized, 0, BUFFER_OFFSET(block_offset));

			err = glGetError();
			printf("Setting up VAA %d (size = %d, type = %d, normalized = %d, offset = %ld) ERR = %d\n",
					vaa, size, accessor.componentType, accessor.normalized, block_offset, err);

			block_offset += block_size;
		
		}
		
		// Buffer mesh indices
		tinygltf::Accessor i_accessor = model.accessors[prim.indices];
		tinygltf::BufferView i_bufView = model.bufferViews[i_accessor.bufferView];
		tinygltf::Buffer i_buf = model.buffers[i_bufView.buffer];
		size_t i_offset = i_bufView.byteOffset + i_accessor.byteOffset;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_accessor.count * i_accessor.ByteStride(i_bufView), 
				&i_buf.data.at(0) + i_offset, GL_STATIC_DRAW);
		
		glBindVertexArray(0);

		return mesh_resource(vao, vbo, ibo, vertex_count, i_accessor.count);

	}

	void gltf_model_loader::parse_node(tinygltf::Model& model, tinygltf::Node& node)
	{
		if (node.mesh >= 0 && node.mesh < model.meshes.size())
		{
			mesh_resource res = build_mesh(model, model.meshes[node.mesh]);
			this->mesh = res;
		}

		for (size_t i = 0; i < node.children.size(); i++) 
		{
			assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
			parse_node(model, model.nodes[node.children[i]]);
		}
	}

	unsigned gltf_model_loader::get_meshes(tinygltf::Model& model)
	{
		const tinygltf::Scene scene = model.scenes[model.defaultScene];

		printf("Parsing GLTF file, %lu nodes in default scene %d\n", scene.nodes.size(), model.defaultScene);

		for (int i = 0; i < scene.nodes.size(); i++)
		{
			tinygltf::Node& node = model.nodes[scene.nodes[i]];
			parse_node(model, node);
		}

		return 0;
	}

	gltf_model_loader::~gltf_model_loader()
	{
		printf("GLTF loader exited");
	}
}

