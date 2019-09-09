#pragma once

#include "GL/glew.h"

#include "..\..\projects\VectorLib\code\matrix4.h"

namespace efiilj
{
	class MeshResource
	{
	private:
		GLuint vao;
		GLuint vbo;
		GLuint ibo;
		int vertexCount;

	public:

		MeshResource();
		MeshResource(float* vertexList, unsigned int* indexList, int count);

		void InitVertexBuffer(float* vertexList, int count);
		void InitIndexBuffer(int* indexList, int count);
		void InitArrayObject();
		void Draw(const GLuint shader);
	};
}