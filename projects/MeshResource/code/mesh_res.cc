#include "mesh_res.h"

#include <GL/glew.h>

namespace efiilj
{
	mesh_resource::mesh_resource() : vbo_(0), ibo_(0), vao_(0), vertex_count_(0), index_count_(0)
	{
		init_array_object();
		glGenBuffers(1, &this->vbo_);
		glGenBuffers(1, &this->ibo_);
		unbind();
	}

	mesh_resource::
	mesh_resource(vertex* vertex_list, const int vertex_count, unsigned int* index_list, const int index_count) : vbo_(0), ibo_(0), vao_(0)
	{
		this->vertex_count_ = vertex_count;
		this->index_count_ = index_count;

		init_array_object();
		init_vertex_buffer(vertex_list, vertex_count);
		init_index_buffer(index_list, index_count);
	}

	mesh_resource::mesh_resource(unsigned vao, unsigned vbo, unsigned ibo, int vertex_count, int index_count) 
		: vao_(vao), vbo_(vbo), ibo_(ibo), vertex_count_(vertex_count), index_count_(index_count)  {}

	mesh_resource mesh_resource::cube(float size, const float color)
	{
		size /= 2;

		vertex vertices[24] = {

			// Front = S
			vertex(vector3(-size, -size, size), vector3(0, 0, 1), vector4(color, color, color, color), vector2(0.25f, 0.33f)),	// 0
			vertex(vector3(size, -size, size), vector3(0, 0, 1), vector4(color, color, color, color), vector2(0.5f, 0.33f)),		// 1
			vertex(vector3(size, size, size), vector3(0, 0, 1), vector4(color, color, color, color), vector2(0.5f, 0.66f)),		// 2
			vertex(vector3(-size, size, size), vector3(0, 0, 1), vector4(color, color, color, color), vector2(0.25f, 0.66f)), // 3

			// Back = N
			vertex(vector3(size, -size, -size), vector3(0, 0, -1), vector4(color, color, color, color), vector2(0.75, 0.33f)), // 4
			vertex(vector3(-size, -size, -size), vector3(0, 0, -1), vector4(color, color, color, color), vector2(1.0f, 0.33f)), // 5
			vertex(vector3(-size, size, -size), vector3(0, 0, -1), vector4(color, color, color, color), vector2(1.0f, 0.66f)), // 6
			vertex(vector3(size, size, -size), vector3(0, 0, -1), vector4(color, color, color, color), vector2(0.75, 0.66f)), // 7

			// Top
			vertex(vector3(-size, size, size), vector3(0, 1, 0), vector4(color, color, color, color), vector2(0.25f, 0.66f)), // 8
			vertex(vector3(size, size, size), vector3(0, 1, 0), vector4(color, color, color, color), vector2(0.5f, 0.66f)), // 9
			vertex(vector3(size, size, -size), vector3(0, 1, 0), vector4(color, color, color, color), vector2(0.5f, 1.0f)), // 10
			vertex(vector3(-size, size, -size), vector3(0, 1, 0), vector4(color, color, color, color), vector2(0.25f, 1.0f)), // 11

			// Bottom
			vertex(vector3(size, -size, size), vector3(0, -1, 0), vector4(color, color, color, color), vector2(0.25f, 0.0f)), // 12
			vertex(vector3(-size, -size, size), vector3(0, -1, 0), vector4(color, color, color, color), vector2(0.5f, 0.0f)), // 13
			vertex(vector3(-size, -size, -size), vector3(0, -1, 0), vector4(color, color, color, color), vector2(0.5f, 0.33f)), // 14
			vertex(vector3(size, -size, -size), vector3(0, -1, 0), vector4(color, color, color, color), vector2(0.25f, 0.33f)), // 15

			// Left = W
			vertex(vector3(-size, -size, -size), vector3(-1, 0, 0), vector4(color, color, color, color), vector2(0.0f, 0.33f)), // 16
			vertex(vector3(-size, -size, size), vector3(-1, 0, 0), vector4(color, color, color, color), vector2(0.25f, 0.33f)), // 17
			vertex(vector3(-size, size, size), vector3(-1, 0, 0), vector4(color, color, color, color), vector2(0.25f, 0.66f)), // 18
			vertex(vector3(-size, size, -size), vector3(-1, 0, 0), vector4(color, color, color, color), vector2(0.0f, 0.66f)), // 19

			// Right = E
			vertex(vector3(size, -size, size), vector3(1, 0, 0), vector4(color, color, color, color), vector2(0.5f, 0.33f)), // 20
			vertex(vector3(size, -size, -size), vector3(1, 0, 0), vector4(color, color, color, color), vector2(0.75, 0.33f)), // 21
			vertex(vector3(size, size, -size), vector3(1, 0, 0), vector4(color, color, color, color), vector2(0.75, 0.66f)), // 22
			vertex(vector3(size, size, size), vector3(1, 0, 0), vector4(color, color, color, color), vector2(0.5f, 0.66f)), // 23
		};

		unsigned int indices[36] = {
			0, 1, 3, 2, 3, 1, // Front
			4, 5, 7, 6, 7, 5, // Back
			8, 9, 11, 10, 11, 9, // Top
			12, 13, 15, 14, 15, 13, // Bottom
			16, 17, 19, 18, 19, 17, // Left
			20, 21, 23, 22, 23, 21 // Right
		};

		return mesh_resource(vertices, 24, indices, 36);
	}

	void mesh_resource::init_vertex_buffer(vertex* vertex_list, const int count)
	{
		if (vbo_ != 0)
			return;

		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(vertex), vertex_list, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, normal)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, rgba)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, uv)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}

	void mesh_resource::init_index_buffer(unsigned int* index_list, const int count)
	{
		if (ibo_ != 0)
			return;

		glGenBuffers(1, &ibo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), index_list, GL_STATIC_DRAW);
	}

	void mesh_resource::init_array_object()
	{
		if (vao_ != 0)
			return;

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);
	}

	void mesh_resource::update_vertex_buffer(vertex* vertex_list) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_count_ * sizeof(vertex), vertex_list);
	}

	void mesh_resource::buffer(unsigned target, size_t size, void* data, unsigned usage)
	{
		bind();
		glBufferData(target, size, data, usage);
		unbind();	
	}

	void mesh_resource::bind() const
	{
		glBindVertexArray(vao_);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
	}

	void mesh_resource::unbind()
	{
		glBindVertexArray(0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void mesh_resource::draw_elements() const
	{
		glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);
	}

	mesh_resource::~mesh_resource()
	{
		std::cout << "Deleting mesh resource " << this << std::endl;

		unbind();
		glDeleteBuffers(1, &vao_);
		glDeleteBuffers(1, &vbo_);
		glDeleteBuffers(1, &ibo_);
	}
}
