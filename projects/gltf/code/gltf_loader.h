#pragma once

#include "tiny_gltf.h"

#include "server.h"
#include "mgr_host.h"

#include "entity.h"
#include "trfm_mgr.h"
#include "cam_mgr.h"
#include "mtrl_mgr.h"
#include "mesh_mgr.h"

#include "tex_srv.h"
#include "mtrl_srv.h"
#include "mesh_srv.h"

#include <string>
#include <memory>

namespace efiilj 
{

	typedef int model_id;

	struct accessor_data
	{
		const unsigned char* data_start;

		size_t stride;
		size_t block_size;
		size_t offset;
		size_t size;
	};

	class gltf_model_server : public server<model_id>
	{
	private:
		
		struct ModelData
		{
			std::vector<std::filesystem::path> uri;	
			std::vector<tinygltf::Model> model;
			std::vector<bool> binary;
			std::vector<bool> open;
		} _data;

		std::shared_ptr<entity_manager> _entities;

		std::shared_ptr<mesh_server> _meshes;
		std::shared_ptr<texture_server> _textures;
		std::shared_ptr<material_server> _materials;
		
		std::shared_ptr<material_manager> _material_instances;
		std::shared_ptr<mesh_manager> _mesh_instances;
		std::shared_ptr<transform_manager> _transforms;
		std::shared_ptr<camera_manager> _cameras;

		size_t calculate_vbo_size(const tinygltf::Model& model, const tinygltf::Primitive& prim);
		accessor_data calculate_accessor_data(const tinygltf::Model& model, const tinygltf::Accessor& acc);
		matrix4 convert_matrix(const std::vector<double>& m);

		mesh_id add_primitive(entity_id eid, model_id idx, const tinygltf::Primitive& prim);
		camera_id add_camera(entity_id eid, const tinygltf::Camera& cam);
		void get_mesh(entity_id eid, model_id idx, const tinygltf::Mesh& mesh);
		void get_node(model_id idx, const tinygltf::Node&);
		void link_texture(model_id idx, material_id mat_id, int tex_index, const texture_type& usage);
		material_id get_material(model_id idx, const tinygltf::Material& mat);

	public:

		gltf_model_server();
		~gltf_model_server();

		void append_defaults(model_id) override;
		void on_register(std::shared_ptr<manager_host> host) override;

		bool load(model_id idx);
		bool unload(model_id idx);

		bool get_nodes(model_id idx);
		bool get_cameras(model_id idx, entity_id eid);
		bool get_lights(model_id idx, entity_id eid);

		const std::filesystem::path& get_uri(model_id idx) const
		{
			return _data.uri[idx];
		}

		void set_uri(model_id idx, const std::filesystem::path& uri)
		{
			_data.uri[idx] = uri;
		}
	};
}
