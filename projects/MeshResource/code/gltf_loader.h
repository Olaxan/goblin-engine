#pragma once

#include "tiny_gltf.h"
#include "mesh_res.h"
#include "program.h"
#include "node.h"
#include "material.h"

#include <string>
#include <memory>

namespace efiilj 
{
	class gltf_model_loader
	{
	private:
		void build_mesh(tinygltf::Model&, tinygltf::Mesh&);
		void parse_node(tinygltf::Model&, tinygltf::Node&);
		void link_texture(tinygltf::Model&, gltf_pbr_base& mat, int index, const std::string& type);
		unsigned get_meshes(tinygltf::Model&);
		unsigned get_materials(tinygltf::Model& model);
		
		mesh_resource mesh;
		std::vector<graphics_node> nodes_;
		std::vector<gltf_pbr_base> materials_;
		std::shared_ptr<shader_program> shader_;

	public:
		gltf_model_loader();
		gltf_model_loader(const std::string& path, std::shared_ptr<shader_program> shader);

		gltf_model_loader(gltf_model_loader&)
			= default;

		gltf_model_loader(gltf_model_loader&&)
			= default;
	
		static size_t type_component_count(const std::string& type);
		static size_t component_type_size(int type);
		static unsigned get_format(int components);
		static unsigned get_type(int bits);

		bool load_from_file(tinygltf::Model& model, const std::string& path);

		void draw() const;

		mesh_resource get_resource()
		{
			return mesh;
		}
	
		~gltf_model_loader();
	};
}
