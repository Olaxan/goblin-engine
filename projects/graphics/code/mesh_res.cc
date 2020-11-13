#include "mesh_res.h"

#include <GL/glew.h>

namespace efiilj
{
	mesh_resource::mesh_resource() : type_(GL_UNSIGNED_INT), vbo_(0), ibo_(0), vao_(0), vertex_count_(0), index_count_(0)
	{ }

	mesh_resource::
	mesh_resource(vertex* vertex_list, const int vertex_count, unsigned int* index_list, const int index_count) 
	: type_(GL_UNSIGNED_INT), vbo_(0), ibo_(0), vao_(0), vertex_count_(vertex_count), index_count_(index_count)
	{
		init_array_object();
		init_vertex_buffer(vertex_list, vertex_count);
		init_index_buffer(index_list, index_count);
	}

	mesh_resource::mesh_resource(unsigned type, unsigned vao, unsigned vbo, unsigned ibo, int vertex_count, int index_count) 
		: type_(type), vao_(vao), vbo_(vbo), ibo_(ibo), vertex_count_(vertex_count), index_count_(index_count)  {}

	void mesh_resource::init_vertex_buffer(vertex* vertex_list, const int count)
	{
		if (vbo_ != 0)
			return;

		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(vertex), vertex_list, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, normal)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, uv)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
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
	}

	void mesh_resource::unbind()
	{
		glBindVertexArray(0);
	}

	void mesh_resource::draw_elements() const
	{
		glDrawElements(GL_TRIANGLES, index_count_, type_, nullptr);
	}

	mesh_resource::~mesh_resource()
	{
		unbind();
		glDeleteBuffers(1, &vao_);
		glDeleteBuffers(1, &vbo_);
		glDeleteBuffers(1, &ibo_);
	}
}
