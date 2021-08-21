#include "mesh_res.h"

#include <GL/glew.h>

namespace efiilj
{
	mesh_resource::mesh_resource() 
		: _type(GL_UNSIGNED_INT), _usage(GL_STATIC_DRAW), _vbo(0), _ibo(0), _vao(0), material_index(-1)
	{ }

	void mesh_resource::finalize()
	{

		init_array_object();
		init_vertex_buffer();
		init_index_buffer();

		size_t stride = 0;

		stride += has_position_data() * sizeof(vector3);
		stride += has_normal_data()  * sizeof(vector3);
		stride += has_uv_data() * sizeof(vector2);
		stride += has_tangent_data() * sizeof(vector4);

		glBufferData(GL_ARRAY_BUFFER, get_vertex_count() * stride, NULL, _usage);

		size_t offset = 0;
		if (has_position_data())
		{
			size_t size = _positions.size() * sizeof(vector3);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, _positions.data());
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			offset += size;
		}

		if (has_normal_data())
		{
			size_t size = _normals.size() * sizeof(vector3);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, _normals.data());
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			offset += size;
		}

		if (has_uv_data())
		{
			size_t size = _uvs.size() * sizeof(vector2);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, _uvs.data());
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			offset += size;
		}

		if (has_tangent_data())
		{
			size_t size = _tangents.size() * sizeof(vector4);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, _tangents.data());
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			offset += size;
		}
	}

	void mesh_resource::init_vertex_buffer()
	{
		if (_vbo != 0)
			return;

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	}

	void mesh_resource::init_index_buffer()
	{
		if (_ibo != 0)
			return;

		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_index_count() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);
	}

	void mesh_resource::init_array_object()
	{
		if (_vao != 0)
			return;

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
	}

	void mesh_resource::buffer(unsigned target, size_t size, void* data, unsigned usage)
	{
		bind();
		glBufferData(target, size, data, usage);
		unbind();	
	}

	void mesh_resource::bind() const
	{
		glBindVertexArray(_vao);
	}

	void mesh_resource::unbind()
	{
		glBindVertexArray(0);
	}

	void mesh_resource::clear()
	{
		_positions.clear();
		_normals.clear();
		_uvs.clear();
		_tangents.clear();
		_indices.clear();
	}

	void mesh_resource::draw_elements() const
	{
		glDrawElements(GL_TRIANGLES, get_index_count(), _type, nullptr);
	}

	mesh_resource::~mesh_resource()
	{
		unbind();
		glDeleteBuffers(1, &_vao);
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ibo);
	}
}
