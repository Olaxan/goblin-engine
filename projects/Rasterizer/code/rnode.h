#pragma once

#include "vertex.h"
#include "transform.h"

#include <vector>
#include <functional>


namespace efiilj
{
	struct vertex_uniforms
	{
		matrix4 camera;
		matrix4 model;
	};

	struct vertex_data
	{
		vector4 fragment;
		vector4 normal;
		vector4 color;
		vector2 uv;
	};

	struct fragment_uniforms
	{
		
	};
	
	class rasterizer_node
	{
	private:
		std::vector<vertex> vertices_;
		std::vector<unsigned> indices_;
		std::shared_ptr<transform_model> transform_;

	public:
		rasterizer_node(std::vector<vertex> vertices, std::vector<unsigned> indices, std::shared_ptr<transform_model> transform);

		std::function<vertex_data(vertex&, vertex_uniforms)> vertex_shader;
		std::function<unsigned(vector2, vector4, fragment_uniforms)> fragment_shader; //UV, Normal, Texture

		unsigned int vertex_count() const { return vertices_.size(); }
		unsigned int index_count() const { return indices_.size(); }

		vertex& get_by_index(const unsigned index) { return vertices_[indices_[index]]; }

		transform_model& transform() const { return *this->transform_; }
		void transform(std::shared_ptr<transform_model>& transform) { this->transform_ = std::move(transform); }
	};
}
