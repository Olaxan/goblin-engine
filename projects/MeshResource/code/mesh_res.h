#pragma once

#include "vertex.h"

namespace efiilj
{
	
	/**
	 * \brief Class to create a mesh on the GPU, as well as to hold buffer handles which enables binding.
	 */
	class mesh_resource
	{
	private:

		/**
		 * \brief Vertex buffer object
		 */
		unsigned int vbo_;
		/**
		 * \brief Index buffer object
		 */
		unsigned int ibo_;
		/**
		 * \brief Vertex array object
		 */
		unsigned int vao_;

		int vertex_count_;
		int index_count_;

		/**
		 * \brief Creates and initializes the Vertex Buffer, configures vertex attribute pointers, and enables attribute arrays.
		 *  Ensure a Vertex Array Object has been configured and bound before running this function.
		 * \param vertex_list The list of vertices to buffer
		 * \param count Size of vertex list
		 */
		void init_vertex_buffer(vertex* vertex_list, int count);

		/**
		 * \brief Creates and initializes the Index Buffer.
		 * \param index_list The list of indices to buffer
		 * \param count Size of index list
		 */
		void init_index_buffer(unsigned int* index_list, int count);

		/**
		 * \brief Creates and initializes the Vertex Array Object.
		 */
		void init_array_object();

	public:

		/**
		 * \brief Creates an empty MeshResource object.
		 */
		mesh_resource();

		/**
		 * \brief Creates a new MeshResource instance with the specified vertex and index lists.
		 * \param vertex_list List of vertices to buffer
		 * \param vertex_count Size of the vertex list
		 * \param index_list List of indices to buffer
		 * \param index_count Size of the index list
		 */
		mesh_resource(vertex* vertex_list, int vertex_count, unsigned int* index_list, int index_count);

		mesh_resource(unsigned vao, unsigned vbo, unsigned ibo=0);

		mesh_resource(mesh_resource& copy)
			= default;

		mesh_resource(mesh_resource&& move)
			= default;

		/**
		 * \brief Static method to generate a cube of the specified size.
		 * \param size The size across the entire cube
		 * \param color Vertex color
		 * \returns A new Mesh Resource representing a cube
		 */
		static mesh_resource cube(float size, float color = 1);

		/**
		 * \brief Gets mesh vertex count
		 * \returns The number of vertices in the mesh
		 */
		int vertex_count() const
		{
			return vertex_count_;
		}

		/**
		 * \brief Gets mesh index count
		 * \returns The number of indices in the mesh
		 */
		int index_count() const
		{
			return index_count_;
		}

		/**
		 * \brief Binds Vertex Array Object and Index Buffer to prepare OpenGL for drawing this mesh.
		 */
		void bind() const;

		/**
		 * \brief Unbinds vertex, index, and array object buffers on GPU
		 */
		static void unbind();

		void buffer(unsigned target, size_t size, void* data, unsigned usage);

		/**
		 * \brief Pushes a new vertex list (of the same size) to the Vertex Buffer.
		 * \param vertex_list The updated vertex list
		 */
		void update_vertex_buffer(vertex* vertex_list) const;

		/**
		 * \brief Performs a draw call with the correct index specifications.
		 */
		void draw_elements() const;

		~mesh_resource();
	};
}
