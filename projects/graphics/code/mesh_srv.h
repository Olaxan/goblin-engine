#pragma once

#include "server.h"
#include "mgr_host.h"

#include "mtrl_srv.h"
#include "vector4.h"

#include <filesystem>

namespace efiilj
{
	typedef int mesh_id;

	class mesh_server : public server<mesh_id>, public registrable
	{
		private:

			struct MeshData 
			{
				std::vector<std::vector<vector3>> positions;
				std::vector<std::vector<vector3>> normals;
				std::vector<std::vector<vector3>> colors;
				std::vector<std::vector<vector2>> uvs;
				std::vector<std::vector<vector4>> tangents;
				std::vector<std::vector<unsigned>> indices;
				std::vector<vector3> min;
				std::vector<vector3> max;
				std::vector<material_id> material;
				std::vector<unsigned> usage;
				std::vector<unsigned> vao;
				std::vector<unsigned> vbo;
				std::vector<unsigned> ibo;
				std::vector<bool> state;
			} _data;
			
			bool buffer(mesh_id idx);

		public:

			mesh_server();
			~mesh_server();

			mesh_id create() override;
			bool destroy(mesh_id idx) override;

			bool build(mesh_id idx, unsigned usage);
			bool buffer(mesh_id idx);
			bool update(mesh_id idx);

			size_t get_vertex_count(mesh_id idx) const { return _data.positions[idx].size(); }
			size_t get_index_count(mesh_id idx) const { return _data.indices[idx].size(); }

			bool has_position_data(mesh_id idx) const { return _data.positions[idx].size() > 0; }
			bool has_normal_data(mesh_id idx) const { return _data.normals[idx].size() > 0; }
			bool has_color_data(mesh_id idx) const { return _data.colors[idx].size() > 0; }
			bool has_uv_data(mesh_id idx) const { return _data.uvs[idx].size() > 0; }
			bool has_tangent_data(mesh_id idx) const { return _data.tangents[idx].size() > 0; }

			const vector3& get_position(mesh_id idx, size_t index) const 
			{ return _data.positions[idx][index]; }

			const vector3& get_indexed_position(mesh_id idx, size_t index) const 
			{ return get_position(idx, _data.indices[idx][index]); }

			const vector3& get_normal(mesh_id idx, size_t index) const 
			{ return _data.normals[idx][index]; }

			const vector3& get_indexed_normal(mesh_id idx, size_t index) const 
			{ return get_normal(idx, _data.indices[idx][index]); }

			const vector3& get_color(mesh_id idx, size_t index) const 
			{ return _data.colors[idx][index]; }

			const vector3& get_indexed_color(mesh_id idx, size_t index) const 
			{ return get_color(idx, _data.indices[idx][index]); }

			const vector2& get_uv(mesh_id idx, size_t index) const 
			{ return _data.uvs[idx][index]; }

			const vector2& get_indexed_uv(mesh_id idx, size_t index) const 
			{ return get_uv(idx, _data.indices[idx][index]); }

			const vector4& get_tangent(mesh_id idx, size_t index) const 
			{ return _data.tangents[idx][index]; }

			const vector4& get_indexed_tangent(mesh_id idx, size_t index) const 
			{ return get_tangent(idx, _data.indices[idx][index]); }

			void set_positions(mesh_id idx, std::vector<vector3>& positions);
			void set_positions(mesh_id idx, const std::vector<vector3>& positions);

			void set_normals(mesh_id idx, std::vector<vector3>& normals);
			void set_normals(mesh_id idx, const std::vector<vector3>& normals);

			void set_colors(mesh_id idx, std::vector<vector3>& colors);
			void set_colors(mesh_id idx, const std::vector<vector3>& colors);
			
			void set_uvs(mesh_id idx, std::vector<vector2>& uvs);
			void set_uvs(mesh_id idx, const std::vector<vector2>& uvs);

			void set_tangents(mesh_id idx, std::vector<vector4>& tangents);
			void set_tangents(mesh_id idx, const std::vector<vector4>& tangents);

			void set_indices(mesh_id idx, std::vector<unsigned>& indices);
			void set_indices(mesh_id idx, const std::vector<unsigned>& indices);

			void set_bounds(mesh_id idx, const vector3& min, const vector3& max)
			{
				_data.min[idx] = min;
				_data.max[idx] = max;
			}

			void set_material(mesh_id idx, material_id mat_id)
			{
				_data.material[idx] = mat_id;
			}
	};
}
