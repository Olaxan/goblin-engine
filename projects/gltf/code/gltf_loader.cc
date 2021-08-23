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

	gltf_model_loader::gltf_model_loader(std::shared_ptr<transform_manager> trf_mgr)
		: _transforms(std::move(trf_mgr))
	{}

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

	int gltf_model_loader::get_attribute_type(const std::string& name)
	{
		if (name.compare("POSITION") == 0)
			return 0;

		if (name.compare("NORMAL") == 0)
			return 1;

		if (name.compare("TEXCOORD_0") == 0)
			return 2;

		if (name.compare("TANGENT") == 0)
			return 3;

		return -1;
	}

	size_t gltf_model_loader::calculate_vbo_size(tinygltf::Primitive& prim)
	{
		size_t vbo_size = 0;
		for (auto &attrib : prim.attributes)
		{
			tinygltf::Accessor accessor = _model.accessors[attrib.second];
			tinygltf::BufferView view = _model.bufferViews[accessor.bufferView];
			vbo_size += accessor.count * accessor.ByteStride(view); 
		}

		return vbo_size;
	}

	accessor_data gltf_model_loader::calculate_accessor_data(tinygltf::Accessor& accessor)
	{
		tinygltf::BufferView& view = _model.bufferViews[accessor.bufferView];
		tinygltf::Buffer& buf = _model.buffers[view.buffer];

		unsigned char* data_start = &buf.data.at(0);

		size_t stride = accessor.ByteStride(view);
		size_t block_size = stride * accessor.count;
		size_t offset = view.byteOffset + accessor.byteOffset;
		size_t size = (accessor.type == TINYGLTF_TYPE_SCALAR) ? 1 : accessor.type;

		return accessor_data { data_start, stride, block_size, offset, size };
	}

	bool gltf_model_loader::load_from_file(const std::string& path, bool is_binary = false)
	{
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool ret;
		
		if (is_binary)
			ret = loader.LoadBinaryFromFile(&_model, &err, &warn, path); // for binary glTF(.glb)
		else
			ret = loader.LoadASCIIFromFile(&_model, &err, &warn, path); 

		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty()) {
			printf("Err: %s\n", err.c_str());
		}

		return (_model_ready = ret);
	
	}

	std::shared_ptr<scene> gltf_model_loader::get_scene(
				std::shared_ptr<shader_program> program,
				transform_id parent,
				std::string name)
	{
		auto new_scene = std::make_shared<scene>();
		new_scene->name = name;

		if (!_model_ready)
		{
			fprintf(stderr, "ERROR: Model not loaded!\n");
			return new_scene;
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

//				auto trf = std::make_shared<transform_model>();
//				trf->set_parent(parent);

				auto node_ptr = std::make_shared<graphics_node>(_transforms, mesh_ptr, mat_ptr, parent);
				node_ptr->name = name + "_" + std::to_string(i);
				new_scene->nodes.push_back(std::move(node_ptr));
			}
			else fprintf(stderr, "Error: Invalid material assigned to mesh!\n");
		}

		return new_scene;
	}

	std::shared_ptr<mesh_resource> gltf_model_loader::build_mesh(tinygltf::Primitive& prim)
	{

		vector3 pos_min;
		vector3 pos_max;

		auto mesh = std::make_shared<mesh_resource>();
	
		size_t block_offset = 0;
		for (auto &attrib : prim.attributes)
		{
			tinygltf::Accessor accessor = _model.accessors[attrib.second];
			accessor_data data = calculate_accessor_data(accessor);
			int vaa = get_attribute_type(attrib.first);

			switch (vaa)
			{
				case 0:
				{
					auto& max = accessor.maxValues;
					auto& min = accessor.minValues;

					if (max.size() >= 3 && min.size() >= 3)
					{
						pos_max = vector3(max[0], max[1], max[2]);
						pos_min = vector3(min[0], min[1], min[2]);
					}

					assert(accessor.type == 3);

					mesh->_positions.resize(accessor.count);
					memcpy(mesh->_positions.data(), data.data_start + data.offset, data.block_size);
					break;
				}	

				case 1:
				{
					assert(accessor.type == 3);

					mesh->_normals.resize(accessor.count);
					memcpy(mesh->_normals.data(), data.data_start + data.offset, data.block_size);
					break;
				}

				case 2:
				{
					assert(accessor.type == 2);

					mesh->_uvs.resize(accessor.count);
					memcpy(mesh->_uvs.data(), data.data_start + data.offset, data.block_size);
					break;
				}

				case 3:
				{
					assert(accessor.type == 4);

					mesh->_tangents.resize(accessor.count);
					memcpy(mesh->_tangents.data(), data.data_start + data.offset, data.block_size);
					break;
				}

				default:
					printf("GLTF format not supported: %s\n",  attrib.first.c_str());
					break;

			}

			block_offset += data.block_size;
		}

		tinygltf::Accessor i_accessor = _model.accessors[prim.indices];
		accessor_data i_data = calculate_accessor_data(i_accessor);

		unsigned short* temp = new unsigned short[i_accessor.count];
		memcpy(temp, i_data.data_start + i_data.offset, i_data.block_size);
		mesh->_indices.resize(i_accessor.count);

		for (size_t i = 0; i < i_accessor.count; i++)
			mesh->_indices[i] = static_cast<unsigned>(temp[i]);	

		mesh->set_bounds(pos_min, pos_max);
		mesh->material_index = prim.material;

		mesh->build();
		mesh->buffer();

		delete[] temp;
		return mesh;
	}

	void gltf_model_loader::link_texture(std::shared_ptr<gltf_pbr_base> mat, int index, const std::string& type)
	{
		if (index > -1)
		{
			tinygltf::Texture tex_base = _model.textures[index];
			tinygltf::Image src = _model.images[tex_base.source];
			
			unsigned tex_format = get_format(src.component);
			unsigned tex_type = get_type(src.bits);

			mat->add_texture(type, std::make_shared<texture_resource>(src.width, src.height, &src.image.at(0), tex_format, tex_type));
		}
	}

	void gltf_model_loader::parse_node(tinygltf::Node& node, std::shared_ptr<scene> new_scene)
	{
		if (node.mesh >= 0 && node.mesh < _model.meshes.size())
		{
			tinygltf::Mesh mesh = _model.meshes[node.mesh];

			for (auto& prim : mesh.primitives)
			{
				new_scene->meshes.push_back(build_mesh(prim));
			}
		}

		for (size_t i = 0; i < node.children.size(); i++) 
		{
			assert((node.children[i] >= 0) && (node.children[i] < _model.nodes.size()));
			parse_node(_model.nodes[node.children[i]], new_scene);
		}
	}

	unsigned gltf_model_loader::get_meshes(std::shared_ptr<scene> new_scene)
	{
		const tinygltf::Scene scene = _model.scenes[_model.defaultScene];

		printf("Parsing GLTF file, %lu nodes in default scene %d\n", scene.nodes.size(), _model.defaultScene);

		for (int i = 0; i < scene.nodes.size(); i++)
		{
			tinygltf::Node& node = _model.nodes[scene.nodes[i]];
			parse_node(node, new_scene);
		}

		return 0;
	}

	unsigned gltf_model_loader::get_materials(
			std::shared_ptr<shader_program> program,
			std::shared_ptr<scene> new_scene)
	{
		for (int i = 0; i < _model.materials.size(); i++)
		{
			tinygltf::Material mat = _model.materials[i];
			auto pbr_mat = std::make_shared<gltf_pbr_base>(program);

			link_texture(pbr_mat, mat.pbrMetallicRoughness.baseColorTexture.index, "BASE");
			link_texture(pbr_mat, mat.pbrMetallicRoughness.metallicRoughnessTexture.index, "METAL_ROUGHNESS");
			link_texture(pbr_mat, mat.normalTexture.index, "NORMAL");
			link_texture(pbr_mat, mat.emissiveTexture.index, "EMISSIVE");

			auto& emit = mat.emissiveFactor;
			auto& base = mat.pbrMetallicRoughness.baseColorFactor;

			pbr_mat->color = vector4(static_cast<float>(base[0]), static_cast<float>(base[1]), static_cast<float>(base[2]), static_cast<float>(base[3])); 
			pbr_mat->emissive_factor = vector3(static_cast<float>(emit[0]), static_cast<float>(emit[1]), static_cast<float>(emit[2]));
			pbr_mat->metallic_factor = static_cast<float>( mat.pbrMetallicRoughness.metallicFactor);
			pbr_mat->roughness_factor = static_cast<float>( mat.pbrMetallicRoughness.roughnessFactor);
			pbr_mat->double_sided = mat.doubleSided;
			pbr_mat->alpha_cutoff = static_cast<float>(mat.alphaCutoff);

			new_scene->materials.push_back(std::move(pbr_mat));
		}

		return 0;
	}

	gltf_model_loader::~gltf_model_loader()
	{
		printf("GLTF loader exited\n");
	}
}

