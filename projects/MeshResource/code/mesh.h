#pragma once

#include <GL/glew.h>

#include "..\..\projects\VectorLib\code\matrix4.h"

namespace efiilj
{
	class MeshResource
	{
	private:
		GLuint vbo;
		GLuint ibo;
		int vertexCount;

		void InitVertexBuffer(float* vertexList, int count);
		void InitIndexBuffer(unsigned int* indexList, int count);
		void InitArrayObject();

	public:
		GLuint vao;

		MeshResource();
		MeshResource(float* vertexList, unsigned int* indexList, int vertexCount, int indexCount);

		void Bind();
		void Draw(const GLuint shader);
	};
}