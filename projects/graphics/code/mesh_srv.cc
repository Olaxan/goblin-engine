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

	mesh_id mesh_server::create()
	{
		mesh_id new_id = _pool.size();
		_pool.emplace_back(new_id);

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

		return new_id;
	}

	bool mesh_server::destroy(mesh_id idx) //NOLINT
	{
		return false;
	}
	
	bool mesh_server::build(mesh_id idx, unsigned usage)
	{
		if (_data.vao[idx] != 0)
			return false;

		glGenVertexArrays(1, &_data.vao[idx]);
		glBindVertexArray(_data.vao[idx]);
	}

	bool mesh_server::buffer(mesh_id idx)
	{}

	bool mesh_server::update(mesh_id idx)
	{}

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
