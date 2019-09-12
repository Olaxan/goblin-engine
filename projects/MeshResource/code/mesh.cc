#include "mesh.h"

namespace efiilj
{

	MeshResource::MeshResource() : ibo(0), vao(0), vbo(0), indexCount(0), vertexCount(0) { }

	MeshResource::MeshResource(Vertex* vertexList, int vertexCount, unsigned int* indexList, int indexCount)
	{
		this->vertexCount = vertexCount;
		this->indexCount = indexCount;

		ibo = 0;
		vao = 0;
		vbo = 0;

		InitArrayObject();
		InitVertexBuffer(vertexList, vertexCount);
		InitIndexBuffer(indexList, indexCount);
	}

	void MeshResource::InitVertexBuffer(Vertex* vertexList, int count)
	{
		if (vbo != 0)
			return;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), vertexList, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, rgba));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	void MeshResource::InitIndexBuffer(unsigned int* indexList, int count)
	{
		if (ibo != 0)
			return;

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indexList, GL_STATIC_DRAW);
	}

	void MeshResource::InitArrayObject()
	{
		if (vao != 0)
			return;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void MeshResource::UpdateVertexBuffer(Vertex* vertexList)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(Vertex), vertexList);
	}

	void MeshResource::Bind()
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	}

	void MeshResource::Draw(const unsigned int shader)
	{
		glUseProgram(shader);
		Bind();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);
	}
}