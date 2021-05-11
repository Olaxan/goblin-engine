#pragma once

#include "tiny_gltf.h"
#include "program.h"
#include "node.h"
#include "material.h"
#include "camera.h"
#include "scene.h"

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

	class gltf_model_loader
	{
	private:

		void link_texture(std::shared_ptr<gltf_pbr_base> mat, int index, const std::string& type);
		void parse_node(tinygltf::Node&, std::shared_ptr<scene> scene);

		std::shared_ptr<mesh_resource> build_mesh(tinygltf::Primitive&);

		unsigned get_meshes(std::shared_ptr<scene> scene);

		unsigned get_materials(
				std::shared_ptr<shader_program> program,
				std::shared_ptr<scene> scene);
		
		bool load_from_file(tinygltf::Model& model, const std::string& path, bool is_binary);

		size_t calculate_vbo_size(tinygltf::Primitive& prim);
		accessor_data calculate_accessor_data(tinygltf::Accessor&);

		static size_t type_component_count(const std::string& type);
		static size_t component_type_size(int type);
		static unsigned get_format(int components);
		static unsigned get_type(int bits);
		static int get_attribute_type(const std::string&);

		tinygltf::Model model_;
		bool model_ready_;

	public:

		gltf_model_loader(const std::string path, bool is_binary = false);

		gltf_model_loader(gltf_model_loader&)
			= default;

		gltf_model_loader(gltf_model_loader&&)
			= default;

		std::shared_ptr<scene> get_scene(
				std::shared_ptr<shader_program> shader, 
				std::shared_ptr<transform_model> transform,
				std::string name = "GLTF"
		);
	
		~gltf_model_loader();
	};
}
