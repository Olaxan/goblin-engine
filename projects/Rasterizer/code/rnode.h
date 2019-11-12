#pragma once

#include "vertex.h"
#include "transform.h"
#include "swtdata.h"

#include <vector>
#include <functional>
#include "light.h"


namespace efiilj
{
	/**
	 * \brief A data container for uniform values to be passed to a vertex shader.
	 */
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

	/**
	 * \brief A data container for values being returned from a vertex shader.
	 */
	struct vertex_data
	{
		vector4 pos;
		vector4 fragment;
		vector4 normal;
		vector4 color;
		vector2 uv;
	};

	/**
	 * \brief A data container for uniform values being passed to a fragment shader.
	 */
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
	
	/**
	 * \brief A class which represents a simple visual object to be displayed using software rasterization.
	 */
	class rasterizer_node
	{
	private:
		std::vector<vertex> vertices_;
		std::vector<unsigned> indices_;
		std::shared_ptr<transform_model> transform_;
		std::shared_ptr<texture_data> texture_;

	public:
		/**
		 * \brief Creates a new rasterizer node instance.
		 * \param vertices A list of vertices representing the object
		 * \param indices A list of indices for edge sharing
		 * \param transform The position of the object in object-space
		 */
		rasterizer_node(std::vector<vertex> vertices, std::vector<unsigned> indices, std::shared_ptr<transform_model> transform);

		/**
		 * \brief A function pointer for holding a vertex shader.
		 * Runs for each vertex in a face.
		 */
		std::function<vertex_data(vertex*, const vertex_uniforms&)> vertex_shader;
		/**
		 * \brief A function pointer for holding a fragment shader.
		 * Runs for each fragment in a face.
		 */
		std::function<unsigned(const vertex_data & data, const texture_data&, const fragment_uniforms&)> fragment_shader; //UV, Normal, Color Texture

		unsigned int vertex_count() const { return vertices_.size(); }
		unsigned int index_count() const { return indices_.size(); }

		/**
		 * \brief Returns a pointer to a vertex in the buffer.
		 * \param index The index-of-indices representing the vertex which should be returned
		 * \return A pointer to a vertex in the buffer
		 */
		vertex* get_by_index(const unsigned index) { return &vertices_[indices_[index]]; }

		transform_model& transform() const { return *this->transform_; }
		void transform(std::shared_ptr<transform_model>& transform) { this->transform_ = std::move(transform); }

		texture_data& texture() const { return *this->texture_; }
		void texture(std::shared_ptr<texture_data>& texture) { this->texture_ = std::move(texture); }
	};
}
