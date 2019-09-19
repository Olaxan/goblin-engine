#include "mesh_res.h"

#include <GL/glew.h>

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

	MeshResource MeshResource::Cube(float size, float c)
	{
		size /= 2;

		Vertex vertices[36] = {

			// Bottom
			Vertex(Vector3(-size, -size, size), Vector3(0, -1, 0), Vector4(1, 1, 1, 1), Vector2(0.25, 0.33)),			//0
			Vertex(Vector3(size, -size, size), Vector3(0, -1, 0), Vector4(1, 1, 1, 1), Vector2(0.5, 0.33)),				//1
			Vertex(Vector3(-size, -size, -size), Vector3(0, -1, 0), Vector4(1, 1, 1, 1), Vector2(0.25, 0.66)),			//5
			Vertex(Vector3(size, -size, -size), Vector3(0, -1, 0), Vector4(1, 1, 1, 1), Vector2(0.5, 0.66)),			//4
			Vertex(Vector3(-size, -size, -size), Vector3(0, -1, 0), Vector4(1, 1, 1, 1), Vector2(0.25, 0.66)),			//5
			Vertex(Vector3(size, -size, size), Vector3(0, -1, 0), Vector4(1, 1, 1, 1), Vector2(0.5, 0.33)),				//1

			// Top
			Vertex(Vector3(-size, size, size), Vector3(0, 1, 0), Vector4(1, 1, 1, 1), Vector2(0.0, 0.33)),				//2
			Vertex(Vector3(size, size, size), Vector3(0, 1, 0), Vector4(1, 1, 1, 1), Vector2(0.75, 0.33)),				//3
			Vertex(Vector3(-size, size, -size), Vector3(0, 1, 0), Vector4(1, 1, 1, 1), Vector2(0, 0.66)),				//7
			Vertex(Vector3(size, size, -size), Vector3(0, 1, 0), Vector4(1, 1, 1, 1), Vector2(0.75, 0.66)),				//6
			Vertex(Vector3(-size, size, -size), Vector3(0, 1, 0), Vector4(1, 1, 1, 1), Vector2(0, 0.66)),				//7
			Vertex(Vector3(size, size, size), Vector3(0, 1, 0), Vector4(1, 1, 1, 1), Vector2(0.75, 0.33)),				//3
												
			// South 1
			Vertex(Vector3(-size, -size, size), Vector3(0, 0, 1), Vector4(1, 1, 1, 1), Vector2(0.25, 0.33)),			//0
			Vertex(Vector3(size, -size, size), Vector3(0, 0, 1), Vector4(1, 1, 1, 1), Vector2(0.5, 0.33)),				//1
			Vertex(Vector3(-size, size, size), Vector3(0, 0, 1), Vector4(1, 1, 1, 1), Vector2(0.0, 0.33)),				//2
			Vertex(Vector3(size, size, size), Vector3(0, 0, 1), Vector4(1, 1, 1, 1), Vector2(0.75, 0.33)),				//3
			Vertex(Vector3(-size, size, size), Vector3(0, 0, 1), Vector4(1, 1, 1, 1), Vector2(0.0, 0.33)),				//2
			Vertex(Vector3(size, -size, size), Vector3(0, 0, 1), Vector4(1, 1, 1, 1), Vector2(0.5, 0.33)),				//1
												
			// West
			Vertex(Vector3(-size, -size, -size), Vector3(-1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.25, 0.66)), 			//5
			Vertex(Vector3(-size, -size, size), Vector3(-1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.25, 0.33)),			//0
			Vertex(Vector3(-size, size, size), Vector3(-1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.0, 0.33)),				//2
			Vertex(Vector3(-size, size, -size), Vector3(-1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0, 0.66)),				//7
			Vertex(Vector3(-size, size, size), Vector3(-1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.0, 0.33)),				//2
			Vertex(Vector3(-size, -size, -size), Vector3(-1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.25, 0.66)),			//5

			// North
			Vertex(Vector3(size, -size, -size), Vector3(0, 0, -1), Vector4(1, 1, 1, 1), Vector2(0.5, 0.66)),			//4
			Vertex(Vector3(-size, -size, -size), Vector3(0, 0, -1), Vector4(1, 1, 1, 1), Vector2(0.25, 0.66)),			//5
			Vertex(Vector3(size, size, -size), Vector3(0, 0, -1), Vector4(1, 1, 1, 1), Vector2(0.75, 0.66)),			//6
			Vertex(Vector3(-size, size, -size), Vector3(0, 0, -1), Vector4(1, 1, 1, 1), Vector2(0, 0.66)),				//7
			Vertex(Vector3(size, size, -size), Vector3(0, 0, -1), Vector4(1, 1, 1, 1), Vector2(0.75, 0.66)),			//6
			Vertex(Vector3(-size, -size, -size), Vector3(0, 0, -1), Vector4(1, 1, 1, 1), Vector2(0.25, 0.66)),			//5

			// East
			Vertex(Vector3(size, -size, size), Vector3(1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.5, 0.33)), 				//1
			Vertex(Vector3(size, -size, -size), Vector3(1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.5, 0.66)),				//4
			Vertex(Vector3(size, size, size), Vector3(1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.75, 0.33)),				//3
			Vertex(Vector3(size, size, -size), Vector3(1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.75, 0.66)),				//6
			Vertex(Vector3(size, size, size), Vector3(1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.75, 0.33)),				//3
			Vertex(Vector3(size, -size, -size), Vector3(1, 0, 0), Vector4(1, 1, 1, 1), Vector2(0.5, 0.66)),				//4
		};

		unsigned int indices[36] = {
			0, 1, 5, 4, 5, 1,
			2, 3, 7, 6, 7, 3,
			0, 1, 2, 3, 2, 1,
			5, 0, 2, 7, 2, 5,
			4, 5, 6, 7, 6, 5,
			1, 4, 3, 6, 3, 4
		};

		return MeshResource(vertices, 36, indices, 36);
	}

	void MeshResource::InitVertexBuffer(Vertex* vertexList, int count)
	{
		if (vbo != 0)
			return;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), vertexList, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, rgba));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
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

	void MeshResource::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void MeshResource::Draw(const unsigned int shader)
	{
		glUseProgram(shader);
		Bind();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);
	}

	MeshResource::~MeshResource()
	{
		//todo
	}
}