#include "gltf_loader.h"
#include "gltf_exts.h"

#define TINYGLTF_IMPLEMENTATION

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "tiny_gltf.h"

#include <stdio.h>
#include <string.h>
#include <GL/glew.h>

#include <memory>
#include <vector>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace gltfe = exts::gltf;

namespace efiilj
{

	gltf_model_server::gltf_model_server()
	{
		printf("Init gltf...\n");
	}

	gltf_model_server::~gltf_model_server()
	{
		printf("GLTF server exited\n");
	}


	model_id gltf_model_server::create()
	{
		model_id new_id = _pool.size();
		_pool.emplace_back(new_id);

		_data.uri.emplace_back();
		_data.model.emplace_back();
		_data.open.emplace_back(false);
		_data.binary.emplace_back(false);

		return new_id;
	}

	bool gltf_model_server::destroy(model_id idx) //NOLINT
	{
		return false;
	}
	
	void gltf_model_server::on_register(std::shared_ptr<manager_host> host)
	{
		_entities = host->get_manager_from_fcc<entity_manager>('ENTS');

		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');
		_materials = host->get_manager_from_fcc<material_server>('MASR');
		_textures = host->get_manager_from_fcc<texture_server>('TXSR');

		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
		_cameras = host->get_manager_from_fcc<camera_manager>('CAMS');
		_mesh_instances = host->get_manager_from_fcc<mesh_manager>('MEMR');
		_material_instances = host->get_manager_from_fcc<material_manager>('MAMR');
	}

	bool gltf_model_server::load(model_id idx)
	{
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool ret;
		
		if (_data.binary[idx])
			ret = loader.LoadBinaryFromFile(&_data.model[idx], &err, &warn, _data.uri[idx]); 
		else
			ret = loader.LoadASCIIFromFile(&_data.model[idx], &err, &warn, _data.uri[idx]); 

		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty()) {
			printf("Err: %s\n", err.c_str());
		}

		return (_data.open[idx] = ret);
	
	}

	bool gltf_model_server::unload(model_id idx)
	{
		if (!_data.open[idx])
			return true;

		return false;
	}

	size_t gltf_model_server::calculate_vbo_size(const tinygltf::Model& model, const tinygltf::Primitive& prim)
	{
		size_t vbo_size = 0;
		for (auto &attrib : prim.attributes)
		{
			tinygltf::Accessor accessor = model.accessors[attrib.second];
			tinygltf::BufferView view = model.bufferViews[accessor.bufferView];
			vbo_size += accessor.count * accessor.ByteStride(view); 
		}

		return vbo_size;
	}

	accessor_data gltf_model_server::calculate_accessor_data(
			const tinygltf::Model& model, const tinygltf::Accessor& accessor)
	{
		const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
		const tinygltf::Buffer& buf = model.buffers[view.buffer];

		const unsigned char* data_start = &buf.data.at(0);

		size_t stride = accessor.ByteStride(view);
		size_t block_size = stride * accessor.count;
		size_t offset = view.byteOffset + accessor.byteOffset;
		size_t size = (accessor.type == TINYGLTF_TYPE_SCALAR) ? 1 : accessor.type;

		return accessor_data { data_start, stride, block_size, offset, size };
	}

	matrix4 gltf_model_server::convert_matrix(const std::vector<double>& m)
	{
		assert(("Matrix of invalid size!", m.size() == 16)); //NOLINT

		efiilj::matrix4 mat;
		for (size_t i = 0; i < 16; i++)
		{
			mat[i] = static_cast<float>(m[i]);
		}

		return mat;
	}

	mesh_id gltf_model_server::add_primitive(entity_id eid, model_id idx, const tinygltf::Primitive& prim)
	{

		vector3 pos_min;
		vector3 pos_max;

		const tinygltf::Model& model = _data.model[idx];

		mesh_id mid = _meshes->create();
	
		size_t block_offset = 0;
		for (auto &attrib : prim.attributes)
		{

			tinygltf::Accessor accessor = model.accessors[attrib.second];
			accessor_data data = calculate_accessor_data(model, accessor);
			int vaa = gltfe::get_attribute_type(attrib.first);

			switch (vaa)
			{
				case 0:
				{
					auto& max = accessor.maxValues;
					auto& min = accessor.minValues;

					if (max.size() >= 3 && min.size() >= 3)
					{
						pos_max = vector3(
								static_cast<float>(max[0]), 
								static_cast<float>(max[1]), 
								static_cast<float>(max[2]));

						pos_min = vector3(
								static_cast<float>(min[0]), 
								static_cast<float>(min[1]), 
								static_cast<float>(min[2]));

					}

					assert(accessor.type == 3);

					std::vector<vector3> positions(accessor.count);
					memcpy(positions.data(), data.data_start + data.offset, data.block_size);

					_meshes->set_positions(mid, positions);

					break;
				}	

				case 1:
				{
					assert(accessor.type == 3);

					std::vector<vector3> normals(accessor.count);
					memcpy(normals.data(), data.data_start + data.offset, data.block_size);

					_meshes->set_normals(mid, normals);
					break;
				}

				case 2:
				{
					assert(accessor.type == 2);

					std::vector<vector2> uvs(accessor.count);
					memcpy(uvs.data(), data.data_start + data.offset, data.block_size);

					_meshes->set_uvs(mid, uvs);
					break;
				}

				case 3:
				{
					assert(accessor.type == 4);

					std::vector<vector4> tangents(accessor.count);
					memcpy(tangents.data(), data.data_start + data.offset, data.block_size);

					_meshes->set_tangents(mid, tangents);
					break;
				}

				default:
					printf("GLTF format not supported: %s\n",  attrib.first.c_str());
					break;

			}

			block_offset += data.block_size;
		}

		tinygltf::Accessor index_accessor = model.accessors[prim.indices];
		accessor_data index_data = calculate_accessor_data(model, index_accessor);

		unsigned short* temp = new unsigned short[index_accessor.count];
		memcpy(temp, index_data.data_start + index_data.offset, index_data.block_size);

		std::vector<unsigned> indices(index_accessor.count);

		for (size_t i = 0; i < index_accessor.count; i++)
			indices[i] = static_cast<unsigned>(temp[i]);	

		_meshes->set_indices(mid, indices);
		delete[] temp;

		_meshes->set_bounds(mid, pos_min, pos_max);

		if (prim.material > -1)
		{
			const tinygltf::Material& mat = model.materials[prim.material];
			material_id mat_id = get_material(idx, mat);
			
			material_instance_id mat_iid = _material_instances->register_entity(eid);
			_material_instances->set_material(mat_iid, mat_id);
		}

		mesh_instance_id miid = _mesh_instances->register_entity(eid);
		_mesh_instances->set_mesh(miid, mid);

		return mid;
	}


	void gltf_model_server::get_mesh(entity_id eid, model_id idx, const tinygltf::Mesh& mesh)
	{
		for (const auto& prim : mesh.primitives)
		{
			mesh_id mid = add_primitive(eid, idx, prim);
			_meshes->build(mid, GL_STATIC_DRAW);
		}
	}

	camera_id gltf_model_server::add_camera(entity_id eid, const tinygltf::Camera& cam)
	{
		camera_id cam_id = _cameras->register_entity(eid);

		// TODO: find transform
		
		_cameras->set_fov(cam_id, static_cast<float>(cam.perspective.yfov));
		_cameras->set_near(cam_id, static_cast<float>(cam.perspective.znear));
		_cameras->set_far(cam_id, static_cast<float>(cam.perspective.zfar));

		return cam_id;
	}

	void gltf_model_server::get_node(model_id idx, const tinygltf::Node& node)
	{

		const tinygltf::Model& model = _data.model[idx];

		entity_id eid = _entities->create();

		transform_id trf_id = _transforms->register_entity(eid);

		if (node.translation.size() == 3)
		{
			_transforms->set_position(trf_id, vector3(
						static_cast<float>(node.translation[0]),
						static_cast<float>(node.translation[1]),
						static_cast<float>(node.translation[2])));
		}

		if (node.rotation.size() == 3)
		{
			_transforms->set_scale(trf_id, vector3(
						static_cast<float>(node.scale[0]),
						static_cast<float>(node.scale[1]),
						static_cast<float>(node.scale[2])));
		}

		// TODO: rotation
		
		if (node.camera > -1)
		{
			add_camera(eid, model.cameras[node.camera]);
		}

		if (node.mesh > -1)
		{
			get_mesh(eid, idx, model.meshes[node.mesh]);
		}

		for (size_t i = 0; i < node.children.size(); i++) 
		{
			get_node(idx, model.nodes[node.children[i]]);
		}
	}

	bool gltf_model_server::get_nodes(model_id idx)
	{

		if (!is_valid(idx) || !_data.open[idx])
			return false;

		const tinygltf::Model& model = _data.model[idx];
		const tinygltf::Scene& scene = _data.model[idx].scenes[model.defaultScene];

		printf("Parsing GLTF file, %lu nodes in default scene %d\n", scene.nodes.size(), model.defaultScene);

		for (size_t i = 0; i < scene.nodes.size(); i++)
		{
			const tinygltf::Node& node = model.nodes[scene.nodes[i]];
			get_node(idx, node);
		}

		return 0;
	}

	void gltf_model_server::link_texture(model_id idx, material_id mat_id, int tex_index, const texture_type& type)
	{
		if (tex_index > -1 && _materials->is_valid(mat_id))
		{

			const tinygltf::Model& model = _data.model[idx];
			tinygltf::Texture tex_base = model.textures[tex_index];
			tinygltf::Image src = model.images[tex_base.source];
			
			unsigned tex_format = gltfe::get_format(src.component);
			unsigned tex_type = gltfe::get_type(src.bits);

			texture_id tex_id = _textures->create();

			_textures->generate(tex_id);
			_textures->set_format(tex_id, tex_format);
			_textures->set_type(tex_id, tex_type);
			_textures->buffer(tex_id, src.width, src.height, &src.image.at(0));
			_textures->set_usage(tex_id, type);

			_materials->add_texture(mat_id, tex_id);
		}
	}

	material_id gltf_model_server::get_material(model_id idx, const tinygltf::Material& mat)
	{
		material_id mat_id = _materials->create();

		link_texture(idx, mat_id, mat.pbrMetallicRoughness.baseColorTexture.index, texture_type::tex_base);
		link_texture(idx, mat_id, mat.pbrMetallicRoughness.metallicRoughnessTexture.index, texture_type::tex_orm);
		link_texture(idx, mat_id, mat.normalTexture.index, texture_type::tex_normal);
		link_texture(idx, mat_id, mat.emissiveTexture.index, texture_type::tex_emissive);

		auto& emit = mat.emissiveFactor;
		auto& base = mat.pbrMetallicRoughness.baseColorFactor;

		_materials->set_base_color(mat_id, vector4(
					static_cast<float>(base[0]), 
					static_cast<float>(base[1]), 
					static_cast<float>(base[2]), 
					static_cast<float>(base[3]))); 

		_materials->set_emissive_factor(mat_id, vector3(
					static_cast<float>(emit[0]), 
					static_cast<float>(emit[1]), 
					static_cast<float>(emit[2])));

		_materials->set_metallic_factor(mat_id, 
			static_cast<float>( mat.pbrMetallicRoughness.metallicFactor));

		_materials->set_roughness_factor(mat_id, 
			static_cast<float>( mat.pbrMetallicRoughness.roughnessFactor));

		_materials->set_double_sided(mat_id, mat.doubleSided);

		_materials->set_alpha_cutoff(mat_id, static_cast<float>(mat.alphaCutoff));

		return mat_id;
	}
}

