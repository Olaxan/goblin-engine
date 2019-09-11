#pragma once

#include <GL/glew.h>

#include "matrix4.h"

namespace efiilj
{
	struct Vertex
	{
		Vertex(float x, float y, float z, float r, float g, float b, float a) : xyz(x, y, z), rgba(r, g, b, a) { }

		Vector3 xyz;
		Vector4 rgba;
	};

	class MeshResource
	{
	private:
		unsigned int vbo;
		unsigned int ibo;
		int vertexCount;
		int indexCount;

		void InitVertexBuffer(Vertex* vertexList, int count);

		void InitIndexBuffer(unsigned int* indexList, int count);
		void InitArrayObject();

	public:
		unsigned int vao;

		MeshResource();
		MeshResource(Vertex* vertexList, int vertexCount, unsigned int* indexList, int indexCount);

		void Bind();
		void Draw(const GLuint shader);
	};
}