#pragma once

#include <GL/glew.h>

#include "matrix4.h"

namespace efiilj
{
	/// <summary>
	/// Struct to represent a single vertex with position and color.
	/// </summary>
	struct Vertex
	{
		Vertex(float x = 0, float y = 0, float z = 0, float r = 0, float g = 0, float b = 0, float a = 0) : xyzw(x, y, z, 1), rgba(r, g, b, a) { }

		Vector4 xyzw;
		Vector4 rgba;
	};

	/// <summary>
	/// Class to create a mesh on the GPU, as well as to hold buffer handles which enables binding.
	/// </summary>
	class MeshResource
	{
	private:

		/// <summary>
		/// Vertex Buffer Object
		/// </summary>
		unsigned int vbo;

		/// <summary>
		/// Index Buffer Object
		/// </summary>
		unsigned int ibo;

		/// <summary>
		/// Vertex Array Object
		/// </summary>
		unsigned int vao;

		int vertexCount;
		int indexCount;

		/// <summary>
		/// Creates and initializes the Vertex Buffer, configures vertex attribute pointers, and enables attribute arrays.
		/// Ensure a Vertex Array Object has been configured and bound before running this function.
		/// </summary>
		/// <param name="vertexList">The list of vertices to buffer</param>
		/// <param name="count">Size of vertex list</param>
		void InitVertexBuffer(Vertex* vertexList, int count);

		/// <summary>
		/// Creates and initializes the Index Buffer.
		/// </summary>
		/// <param name="indexList">The list of indices to buffer</param>
		/// <param name="count">Size of index list</param>
		void InitIndexBuffer(unsigned int* indexList, int count);

		/// <summary>
		/// Creates and initializes the Vertex Array Object.
		/// </summary>
		void InitArrayObject();

	public:

		/// <summary>
		/// Creates an empty MeshResource object.
		/// </summary>
		MeshResource();

		/// <summary>
		/// Creates a new MeshResource instance with the specified vertex and index lists.
		/// </summary>
		/// <param name="vertexList">List of vertices to buffer</param>
		/// <param name="vertexCount">Size of the vertex list</param>
		/// <param name="indexList">List of indices to buffer</param>
		/// <param name="indexCount">Size of the index list</param>
		MeshResource(Vertex* vertexList, int vertexCount, unsigned int* indexList, int indexCount);

		/// <summary>
		/// Binds Vertex Array Object and Index Buffer to prepare OpenGL for drawing this mesh.
		/// </summary>
		void Bind();

		/// <summary>
		/// Pushes a new vertex list (of the same size) to the Vertex Buffer.
		/// </summary>
		/// <param name="vertexList">The updated vertex list</param>
		void UpdateVertexBuffer(Vertex* vertexList);

		/// <summary>
		/// Shortcut for instantly drawing this mesh with the current settings.
		/// </summary>
		/// <param name="shader">The shader program id to use for drawing</param>
		void Draw(const unsigned int shader);
	};
}