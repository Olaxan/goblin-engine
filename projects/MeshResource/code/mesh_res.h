#pragma once

#include "vertex.h"

namespace efiilj
{
	/// <summary>
	/// Class to create a mesh on the GPU, as well as to hold buffer handles which enables binding.
	/// </summary>
	class mesh_resource
	{
	private:

		/// <summary>
		/// Vertex Buffer Object
		/// </summary>
		unsigned int vbo_;

		/// <summary>
		/// Index Buffer Object
		/// </summary>
		unsigned int ibo_;

		/// <summary>
		/// Vertex Array Object
		/// </summary>
		unsigned int vao_;

		int vertex_count_;
		int index_count_;

		/// <summary>
		/// Creates and initializes the Vertex Buffer, configures vertex attribute pointers, and enables attribute arrays.
		/// Ensure a Vertex Array Object has been configured and bound before running this function.
		/// </summary>
		/// <param name="vertex_list">The list of vertices to buffer</param>
		/// <param name="count">Size of vertex list</param>
		void init_vertex_buffer(vertex* vertex_list, int count);

		/// <summary>
		/// Creates and initializes the Index Buffer.
		/// </summary>
		/// <param name="index_list">The list of indices to buffer</param>
		/// <param name="count">Size of index list</param>
		void init_index_buffer(unsigned int* index_list, int count);

		/// <summary>
		/// Creates and initializes the Vertex Array Object.
		/// </summary>
		void init_array_object();

	public:

		/// <summary>
		/// Creates an empty MeshResource object.
		/// </summary>
		mesh_resource();

		/// <summary>
		/// Creates a new MeshResource instance with the specified vertex and index lists.
		/// </summary>
		/// <param name="vertex_list">List of vertices to buffer</param>
		/// <param name="vertex_count">Size of the vertex list</param>
		/// <param name="index_list">List of indices to buffer</param>
		/// <param name="index_count">Size of the index list</param>
		mesh_resource(vertex* vertex_list, int vertex_count, unsigned int* index_list, int index_count);

		mesh_resource(mesh_resource& copy)
			= default;

		mesh_resource(mesh_resource&& move)
			= default;

		/// <summary>
		/// Static method to generate a cube of the specified size.
		/// </summary>
		/// <param name="size">The size across the entire cube</param>
		/// <param name="color">Vertex color</param>
		/// <returns></returns>
		static mesh_resource cube(float size, float color = 1);

		/// <summary>
		/// Gets mesh vertex count
		/// </summary>
		/// <returns>The number of vertices in the mesh</returns>
		int vertex_count() const
		{
			return vertex_count_;
		}

		/// <summary>
		/// Gets the mesh index count
		/// </summary>
		/// <returns>The number of index in the mesh</returns>
		int index_count() const
		{
			return index_count_;
		}

		/// <summary>
		/// Binds Vertex Array Object and Index Buffer to prepare OpenGL for drawing this mesh.
		/// </summary>
		void bind() const;

		/// <summary>
		/// Unbinds the resource from the OpenGL state machine.
		/// </summary>
		static void unbind();

		/// <summary>
		/// Pushes a new vertex list (of the same size) to the Vertex Buffer.
		/// </summary>
		/// <param name="vertex_list">The updated vertex list</param>
		void update_vertex_buffer(vertex* vertex_list) const;

		/// <summary>
		/// Performs a draw call with the correct index specifications.
		/// </summary>
		void draw_elements() const;

		~mesh_resource();
	};
}
