#pragma once

#include "vertex.h"
#include "transform.h"
#include "swtdata.h"

#include <vector>
#include <functional>
#include "light.h"


namespace efiilj
{
	struct vertex_uniforms
	{
		vertex_uniforms(const matrix4& camera, const matrix4& model)
			: camera(camera), model(model)
		{
			normal = model.inverse().transpose();
		}
		
		matrix4 camera;
		matrix4 model;
		matrix4 normal;
	};

	struct vertex_data
	{
		vector4 pos;
		vector4 fragment;
		vector4 normal;
		vector4 color;
		vector2 uv;
	};

	struct fragment_uniforms
	{
		vector4 normal;
		vector4 fragment;
		
		vector4 camera_position;
		
		vector4 light_rgba;
		vector4 light_intensity;
		vector4 light_position;
		
		vector4 ambient_color;
		float ambient_strength;
		float specular_strength;
		int shininess;
	};
	
	class rasterizer_node
	{
	private:
		std::vector<vertex> vertices_;
		std::vector<unsigned> indices_;
		std::shared_ptr<transform_model> transform_;
		std::shared_ptr<texture_data> texture_;

	public:
		rasterizer_node(std::vector<vertex> vertices, std::vector<unsigned> indices, std::shared_ptr<transform_model> transform);

		std::function<vertex_data(vertex*, const vertex_uniforms&)> vertex_shader;
		std::function<unsigned(const vertex_data & data, const texture_data&, const fragment_uniforms&)> fragment_shader; //UV, Normal, Color Texture

		unsigned int vertex_count() const { return vertices_.size(); }
		unsigned int index_count() const { return indices_.size(); }

		vertex* get_by_index(const unsigned index) { return &vertices_[indices_[index]]; }

		transform_model& transform() const { return *this->transform_; }
		void transform(std::shared_ptr<transform_model>& transform) { this->transform_ = std::move(transform); }

		texture_data& texture() const { return *this->texture_; }
		void texture(std::shared_ptr<texture_data>& texture) { this->texture_ = std::move(texture); }
	};
}
