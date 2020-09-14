#pragma once

#include "tiny_gltf.h"
#include "mesh_res.h"
#include "program.h"
#include "node.h"
#include "material.h"
#include "camera.h"

#include <string>
#include <memory>

namespace efiilj 
{
	class gltf_model_loader
	{
	private:
		void build_mesh(tinygltf::Model&, tinygltf::Mesh&);
		void parse_node(tinygltf::Model&, tinygltf::Node&);
		void link_texture(tinygltf::Model&, std::shared_ptr<gltf_pbr_base> mat, int index, const std::string& type);
		unsigned get_meshes(tinygltf::Model&);
		unsigned get_materials(tinygltf::Model& model);
		
		std::vector<graphics_node> nodes_;
		std::vector<std::shared_ptr<gltf_pbr_base>> materials_;
		std::shared_ptr<shader_program> shader_;
		std::shared_ptr<transform_model> transform_;

	public:
		gltf_model_loader();
		gltf_model_loader(const std::string& path, std::shared_ptr<shader_program> shader, std::shared_ptr<transform_model> transform);

		gltf_model_loader(gltf_model_loader&)
			= default;

		gltf_model_loader(gltf_model_loader&&)
			= default;
	
		static size_t type_component_count(const std::string& type);
		static size_t component_type_size(int type);
		static unsigned get_format(int components);
		static unsigned get_type(int bits);

		bool load_from_file(tinygltf::Model& model, const std::string& path);

		void draw(std::shared_ptr<camera_model> camera) const;
	
		~gltf_model_loader();
	};
}
