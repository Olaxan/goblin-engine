#pragma once

#include "vertex.h"
#include "bounds.h"

#include <memory>

namespace efiilj
{
	
	/**
	 * \brief Class to create a mesh on the GPU, as well as to hold buffer handles which enables binding.
	 */
	class mesh_resource
	{
		protected:

		unsigned vbo_, ibo_, vao_, type_;
		int vertex_count_, index_count_;

		void init_vertex_buffer(vertex* vertex_list, int count);
		void init_index_buffer(unsigned int* index_list, int count);
		void init_array_object();

		bounds bounds_;

	public:

		/**
		 * \brief Creates an empty MeshResource object.
		 */
		mesh_resource();

		mesh_resource(vertex* vertex_list, int vertex_count, unsigned int* index_list, int index_count);
		mesh_resource(unsigned type, unsigned vao, unsigned vbo, unsigned ibo=0, int vertex_count=-1, int index_count=-1);

		unsigned vao() const { return this->vao_; }
		unsigned vbo() const { return this->vbo_; }
		unsigned ibo() const { return this->ibo_; }

		int vertex_count() const { return vertex_count_; }
		int index_count() const { return index_count_; }

		void bind() const;
		void unbind();

		virtual void draw_elements() const;

		void set_bounds(const vector3& min, const vector3& max) { bounds_ = bounds(min,  max); }
		void set_bounds(const bounds& bounds) { bounds_ = bounds; }
		const bounds& get_bounds() { return bounds_; }
		bounds get_bounds(const matrix4& mat) { return bounds_.get_transformed_bounds(mat); }

		void buffer(unsigned target, size_t size, void* data, unsigned usage);

		/**
		 * \brief Pushes a new vertex list (of the same size) to the Vertex Buffer.
		 * \param vertex_list The updated vertex list
		 */
		void update_vertex_buffer(vertex* vertex_list) const;


		~mesh_resource();
	};
}
