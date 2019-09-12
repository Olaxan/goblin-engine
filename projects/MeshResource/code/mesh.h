#pragma once

#include <GL/glew.h>

#include "matrix4.h"

namespace efiilj
{
	struct Vertex
	{
		Vertex(float x = 0, float y = 0, float z = 0, float r = 0, float g = 0, float b = 0, float a = 0) : xyzw(x, y, z, 1), rgba(r, g, b, a) { }

		Vector4 xyzw;
		Vector4 rgba;
	};

	class MeshResource
	{
	private:
		unsigned int vbo;
		unsigned int ibo;
		unsigned int vao;
		int vertexCount;
		int indexCount;

		void InitVertexBuffer(Vertex* vertexList, int count);
		void InitIndexBuffer(unsigned int* indexList, int count);
		void InitArrayObject();

	public:

		MeshResource();
		MeshResource(Vertex* vertexList, int vertexCount, unsigned int* indexList, int indexCount);

		void Bind();
		void UpdateVertexBuffer(Vertex* vertexList);
		void Draw(const unsigned int shader);
	};
}