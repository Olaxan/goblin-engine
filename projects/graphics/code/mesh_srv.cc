#include "mesh_srv.h"

#include <GL/glew.h>

namespace efiilj
{
	
	mesh_server::mesh_server()
	{
		printf("Init mesh...\n");
	}

	mesh_server::~mesh_server()
	{
		printf("Mesh server exited\n");
	}

	void mesh_server::append_defaults(mesh_id)
	{
		_data.positions.emplace_back();
		_data.normals.emplace_back();
		_data.colors.emplace_back();
		_data.uvs.emplace_back();
		_data.tangents.emplace_back();
		_data.indices.emplace_back();
		_data.min.emplace_back(vector3(0));
		_data.max.emplace_back(vector3(0));
		_data.material.emplace_back(-1);
		_data.usage.emplace_back(0);
		_data.vao.emplace_back(0);
		_data.vbo.emplace_back(0);
		_data.ibo.emplace_back(0);
		_data.state.emplace_back(false);
	}

	bool mesh_server::bind(mesh_id idx)
	{
		if (!_data.state[idx])
			return false;

		glBindVertexArray(_data.vao[idx]);
		return true;
	}
	
	bool mesh_server::build(mesh_id idx, unsigned usage)
	{
		if (_data.vao[idx] != 0)
			return false;

		_data.usage[idx] = usage;

		glGenVertexArrays(1, &_data.vao[idx]);
		glBindVertexArray(_data.vao[idx]);

		glGenBuffers(1, &_data.ibo[idx]);
		glGenBuffers(1, &_data.vbo[idx]);

		glBindBuffer(GL_ARRAY_BUFFER, _data.vbo[idx]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _data.ibo[idx]);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
				get_index_count(idx) * sizeof(unsigned int), _data.indices[idx].data(), GL_STATIC_DRAW);

		size_t stride = 0;

		stride += has_position_data(idx) * sizeof(vector3);
		stride += has_normal_data(idx)  * sizeof(vector3);
		stride += has_uv_data(idx) * sizeof(vector2);
		stride += has_tangent_data(idx) * sizeof(vector4);

		glBufferData(GL_ARRAY_BUFFER, get_vertex_count(idx) * stride, NULL, usage);

		buffer(idx);

		_data.state[idx] = true;

		return true;
	}

	bool mesh_server::buffer(mesh_id idx)
	{
		size_t offset = 0;
		if (has_position_data(idx))
		{
			size_t size = _data.positions[idx].size() * sizeof(vector3);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, _data.positions[idx].data());
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			offset += size;
		}

		if (has_normal_data(idx))
		{
			size_t size = _data.normals[idx].size() * sizeof(vector3);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, _data.normals[idx].data());
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			offset += size;
		}

		if (has_uv_data(idx))
		{
			size_t size = _data.uvs[idx].size() * sizeof(vector2);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, _data.uvs[idx].data());
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			offset += size;
		}

		if (has_tangent_data(idx))
		{
			size_t size = _data.tangents[idx].size() * sizeof(vector4);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, _data.tangents[idx].data());
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			offset += size;
		}

		return true;
	}

	void mesh_server::update(mesh_id idx)
	{
		if (_data.usage[idx] == GL_DYNAMIC_DRAW)
			buffer(idx);
	}

	void mesh_server::set_positions(mesh_id idx, std::vector<vector3>& positions)
	{
		_data.positions[idx] = std::move(positions);
	}

	void mesh_server::set_positions(mesh_id idx, const std::vector<vector3>& positions)
	{
		_data.positions[idx] = positions;
	}

	void mesh_server::set_normals(mesh_id idx, std::vector<vector3>& normals)
	{
		_data.normals[idx] = std::move(normals);
	}

	void mesh_server::set_normals(mesh_id idx, const std::vector<vector3>& normals)
	{
		_data.normals[idx] = normals;
	}

	void mesh_server::set_colors(mesh_id idx, std::vector<vector3>& colors)
	{
		_data.colors[idx] = std::move(colors);
	}

	void mesh_server::set_colors(mesh_id idx, const std::vector<vector3>& colors)
	{
		_data.colors[idx] = colors;
	}
	
	void mesh_server::set_uvs(mesh_id idx, std::vector<vector2>& uvs)
	{
		_data.uvs[idx] = std::move(uvs);
	}

	void mesh_server::set_uvs(mesh_id idx, const std::vector<vector2>& uvs)
	{
		_data.uvs[idx] = uvs;
	}

	void mesh_server::set_tangents(mesh_id idx, std::vector<vector4>& tangents)
	{
		_data.tangents[idx] = std::move(tangents);
	}

	void mesh_server::set_tangents(mesh_id idx, const std::vector<vector4>& tangents)
	{
		_data.tangents[idx] = tangents;
	}

	void mesh_server::set_indices(mesh_id idx, std::vector<unsigned>& indices)
	{
		_data.indices[idx] = std::move(indices);
	}

	void mesh_server::set_indices(mesh_id idx, const std::vector<unsigned>& indices)
	{
		_data.indices[idx] = indices;
	}
}
