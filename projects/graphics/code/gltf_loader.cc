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
#include "material.h"

#include <stdio.h>
#include <string.h>
#include <GL/glew.h>

#include <memory>
#include <vector>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace efiilj
{
	gltf_model_loader::gltf_model_loader(const std::string path, bool is_binary)
	{
		model_ready_ = load_from_file(model_, path, is_binary);

		if (model_ready_)
		{
			printf("GLTF file successfully loaded\n");
		}
		else
		{
			printf("Failed to load GLTF\n");
		}
	}

	size_t gltf_model_loader::type_component_count(const std::string& type)
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

	unsigned gltf_model_loader::get_format(int components)
	{
		switch (components)
		{
			case 1:
				return GL_RED;
			case 2:
				return  GL_RG;
			case 3:
				return GL_RGB;
			case 4:
				return GL_RGBA;
			default:
				printf("Error: Unsupported texture format! (%u components)\n", components);
				return 0;
		}
	}
	
	unsigned gltf_model_loader::get_type(int bits)
	{
		switch (bits)
		{
			case 8:
				return GL_UNSIGNED_BYTE;
			case 16:
				return GL_UNSIGNED_SHORT;
			case 32:
				return GL_UNSIGNED_INT;
			default:
				printf("Error: Unsupported texture type! (%u bits)\n", bits);
				return 0;
		}	
	}

	bool gltf_model_loader::load_from_file(tinygltf::Model& model, const std::string& path, bool is_binary = false)
	{
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool ret;
		
		if (is_binary)
			ret = loader.LoadBinaryFromFile(&model, &err, &warn, path); // for binary glTF(.glb)
		else
			ret = loader.LoadASCIIFromFile(&model, &err, &warn, path); 

		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty()) {
			printf("Err: %s\n", err.c_str());
		}

		return ret;	
	
	}

	std::shared_ptr<scene> gltf_model_loader::get_scene(
				std::shared_ptr<shader_program> program,
				std::shared_ptr<transform_model> transform,
				std::string name)
	{
		auto new_scene = std::make_shared<scene>();
		new_scene->name = name;

		if (!model_ready_)
		{
			fprintf(stderr, "ERROR: Model not loaded!\n");
			return new_scene ;
		}

		get_materials(program, new_scene);
		get_meshes(new_scene);

		printf("Retrieved %lu meshes, %lu materials\n", 
				new_scene->meshes.size(), 
				new_scene->materials.size());

		int i = 0;
		for (auto mesh_ptr : new_scene->meshes)
		{
			i++;
			int mat = mesh_ptr->material_index;

			if (mat > -1 && mat < new_scene->materials.size())
			{	
				auto mat_ptr = new_scene->materials[mat];
				auto node_ptr = std::make_shared<graphics_node>(mesh_ptr, mat_ptr, transform);
				node_ptr->name = name + "_" + std::to_string(i);
				new_scene->nodes.push_back(std::move(node_ptr));
			}
			else fprintf(stderr, "Error: Invalid material assigned to mesh!\n");
		}

		return new_scene;
	}

	std::shared_ptr<mesh_resource> gltf_model_loader::build_mesh(tinygltf::Primitive& prim)
	{

		unsigned err;
		unsigned vbo, vao, ibo;

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		vector3 pos_min;
		vector3 pos_max;

		auto data = std::make_shared<mesh_data>();

		// Calculate size of required VBO (to fit all attributes)
		
		size_t vbo_size = 0;
		for (auto &attrib : prim.attributes)
		{
			tinygltf::Accessor accessor = model_.accessors[attrib.second];
			tinygltf::BufferView view = model_.bufferViews[accessor.bufferView];
			vbo_size += accessor.count * accessor.ByteStride(view); 
		}

		glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_STATIC_DRAW);
	
		int vertex_count = -1;
		size_t block_offset = 0;
		for (auto &attrib : prim.attributes)
		{
			tinygltf::Accessor accessor = model_.accessors[attrib.second];
			tinygltf::BufferView& view = model_.bufferViews[accessor.bufferView];
			tinygltf::Buffer& buf = model_.buffers[view.buffer];
			
			size_t stride = accessor.ByteStride(view);
			size_t block_size = stride * accessor.count;
			size_t offset = view.byteOffset + accessor.byteOffset;

			glBufferSubData(GL_ARRAY_BUFFER, block_offset, block_size, &buf.data.at(0) + offset);
			
			int vaa = -1;
			int size = (accessor.type == TINYGLTF_TYPE_SCALAR) ? 1 : accessor.type;
			if (attrib.first.compare("POSITION") == 0)
			{
				vaa = 0;
				vertex_count = accessor.count;

				auto& max = accessor.maxValues;
				auto& min = accessor.minValues;

				if (max.size() >= 3 && min.size() >= 3)
				{
					pos_max = vector3(max[0], max[1], max[2]);
					pos_min = vector3(min[0], min[1], min[2]);
				}

				// Stopgap fix, demand 3 comp. vectors for position
				assert(accessor.type == 3);

				data->positions.resize(accessor.count);
				memcpy(data->positions.data(), &buf.data.at(0) + offset, block_size);
			}	

			if (attrib.first.compare("NORMAL") == 0)
			{
				vaa = 1;

				assert(accessor.type == 3);

				data->normals.resize(accessor.count);
				memcpy(data->normals.data(), &buf.data.at(0) + offset, block_size);
			}

			if (attrib.first.compare("TEXCOORD_0") == 0)
				vaa = 2;

			if (attrib.first.compare("TANGENT") == 0)
				vaa = 3;

			glEnableVertexAttribArray(vaa);
			glVertexAttribPointer(vaa, size, accessor.componentType, accessor.normalized, 0, (void*)block_offset);

			block_offset += block_size;
		
		}

		// Buffer mesh indices
		tinygltf::Accessor i_accessor = model_.accessors[prim.indices];
		tinygltf::BufferView& i_view = model_.bufferViews[i_accessor.bufferView];
		tinygltf::Buffer& i_buf = model_.buffers[i_view.buffer];
		size_t i_offset = i_view.byteOffset + i_accessor.byteOffset;
		size_t i_block_size = i_accessor.count * i_accessor.ByteStride(i_view);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_block_size, 
				&i_buf.data.at(i_offset), GL_STATIC_DRAW);

		unsigned short* temp = new unsigned short[i_accessor.count];
		memcpy(temp, &i_buf.data.at(0) + i_offset, i_block_size);
		data->indices.resize(i_accessor.count);

		for (size_t i = 0; i < i_accessor.count; i++)
		{
			data->indices[i] = static_cast<unsigned>(temp[i]);	
		}

		delete[] temp;

		glBindVertexArray(0);

		auto mesh_ptr = std::make_shared<mesh_resource>(i_accessor.componentType, vao, vbo, ibo, vertex_count, i_accessor.count);
		mesh_ptr->material_index = prim.material;
		mesh_ptr->set_bounds(pos_min, pos_max);
		mesh_ptr->set_mesh_data(data);

		return mesh_ptr;	
	}

	void gltf_model_loader::link_texture(std::shared_ptr<gltf_pbr_base> mat, int index, const std::string& type)
	{
		if (index > -1)
		{
			tinygltf::Texture tex_base = model_.textures[index];
			tinygltf::Image src = model_.images[tex_base.source];
			
			unsigned tex_format = get_format(src.component);
			unsigned tex_type = get_type(src.bits);

			mat->add_texture(type, std::make_shared<texture_resource>(src.width, src.height, &src.image.at(0), tex_format, tex_type));
		}

	}

	void gltf_model_loader::parse_node(tinygltf::Node& node, std::shared_ptr<scene> new_scene)
	{
		if (node.mesh >= 0 && node.mesh < model_.meshes.size())
		{
			tinygltf::Mesh mesh = model_.meshes[node.mesh];

			for (auto& prim : mesh.primitives)
			{
				new_scene->meshes.push_back(build_mesh(prim));
			}
		}

		for (size_t i = 0; i < node.children.size(); i++) 
		{
			assert((node.children[i] >= 0) && (node.children[i] < model_.nodes.size()));
			parse_node(model_.nodes[node.children[i]], new_scene);
		}
	}


	unsigned gltf_model_loader::get_meshes(std::shared_ptr<scene> new_scene)
	{
		const tinygltf::Scene scene = model_.scenes[model_.defaultScene];

		printf("Parsing GLTF file, %lu nodes in default scene %d\n", scene.nodes.size(), model_.defaultScene);

		for (int i = 0; i < scene.nodes.size(); i++)
		{
			tinygltf::Node& node = model_.nodes[scene.nodes[i]];
			parse_node(node, new_scene);
		}

		return 0;
	}

	unsigned gltf_model_loader::get_materials(
			std::shared_ptr<shader_program> program,
			std::shared_ptr<scene> new_scene)
	{
		for (int i = 0; i < model_.materials.size(); i++)
		{
			tinygltf::Material mat = model_.materials[i];
			auto pbr_mat = std::make_shared<gltf_pbr_base>(program);

			link_texture(pbr_mat, mat.pbrMetallicRoughness.baseColorTexture.index, "BASE");
			link_texture(pbr_mat, mat.pbrMetallicRoughness.metallicRoughnessTexture.index, "METAL_ROUGHNESS");
			link_texture(pbr_mat, mat.normalTexture.index, "NORMAL");
			link_texture(pbr_mat, mat.emissiveTexture.index, "EMISSIVE");

			pbr_mat->emissiveFactor = mat.emissiveFactor;
			pbr_mat->baseColorFactor = mat.pbrMetallicRoughness.baseColorFactor;
			pbr_mat->metallicFactor = mat.pbrMetallicRoughness.metallicFactor;
			pbr_mat->roughnessFactor = mat.pbrMetallicRoughness.roughnessFactor;
			pbr_mat->double_sided = mat.doubleSided;

			new_scene->materials.push_back(std::move(pbr_mat));
		}

		return 0;
	}

	gltf_model_loader::~gltf_model_loader()
	{
		printf("GLTF loader exited\n");
	}
}

