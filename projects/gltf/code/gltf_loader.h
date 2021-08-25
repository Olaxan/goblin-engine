#pragma once

#include "tiny_gltf.h"

#include "server.h"
#include "mgr_host.h"

#include "trfm_mgr.h"
#include "tex_srv.h"
#include "mtrl_srv.h"
#include "mesh_srv.h"
#include "cam_mgr.h"

#include <string>
#include <memory>

namespace efiilj 
{
	struct accessor_data
	{
		unsigned char* data_start;

		size_t stride;
		size_t block_size;
		size_t offset;
		size_t size;
	};

	typedef int model_id;

	class gltf_model_server : public server<model_id>, public registrable
	{
	private:

		void link_texture(std::shared_ptr<gltf_pbr_base> mat, int index, const std::string& type);
		void parse_node(tinygltf::Node&, std::shared_ptr<scene> scene);

		std::shared_ptr<mesh_resource> build_mesh(tinygltf::Primitive&);

		unsigned get_meshes(std::shared_ptr<scene> scene);

		unsigned get_materials(
				std::shared_ptr<shader_program> program,
				std::shared_ptr<scene> scene);
		

		size_t calculate_vbo_size(tinygltf::Primitive& prim);
		accessor_data calculate_accessor_data(tinygltf::Accessor&);

		static size_t type_component_count(const std::string& type);
		static size_t component_type_size(int type);
		static unsigned get_format(int components);
		static unsigned get_type(int bits);
		static int get_attribute_type(const std::string&);

		tinygltf::Model _model;
		bool _model_ready;

		struct ModelData
		{
			std::vector<std::filesystem::path> uri;	
			std::vector<tinygltf::Model> model;
			std::vector<bool> binary;
			std::vector<bool> open;
		} _data;

		std::shared_ptr<transform_manager> _transforms;
		std::shared_ptr<camera_manager> _cameras;
		std::shared_ptr<mesh_server> _meshes;
		std::shared_ptr<texture_server> _textures;
		std::shared_ptr<material_server> _materials;

	public:

		gltf_model_server();
		~gltf_model_server();

		model_id create() override;
		bool destroy(model_id idx) override;

		bool load(model_id idx);
		bool unload(model_id idx);

		bool get_materials(model_id idx);
		bool get_cameras(model_id idx);
		bool get_lights(model_id idx);
		bool get_meshes(model_id idx);

		const std::filesystem::path& get_uri(model_id idx) const;
		void set_uri(model_id idx, const std::filesystem::path& uri);
	};
}
