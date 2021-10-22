#pragma once

#include "server.h"
#include "mgr_host.h"

#include "mtrl_srv.h"
#include "vector4.h"
#include "bounds.h"

#include <filesystem>

namespace efiilj
{
	typedef int mesh_id;

	enum class primitive
	{
		line,
		cube,
		bbox
	};

	class mesh_server : public server<mesh_id>
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
				std::vector<bounds> bbox;
				std::vector<vector3> center;
				std::vector<material_id> material;
				std::vector<unsigned> usage;
				std::vector<unsigned> mode;
				std::vector<unsigned> vao;
				std::vector<unsigned> vbo;
				std::vector<unsigned> ibo;
				std::vector<bool> state;
			} _data;

			unsigned int _current_vao;

			void create_line(mesh_id idx);
			void create_cube(mesh_id idx);
			void create_bbox(mesh_id idx);
			
		public:

			mesh_server();
			~mesh_server();

			void append_defaults(mesh_id idx) override;

			mesh_id create_primitive(primitive type);

			bool bind(mesh_id idx);
			bool build(mesh_id idx);
			bool build(mesh_id idx, unsigned usage);
			bool buffer(mesh_id idx);

			void unbind();
			void update(mesh_id idx);
			void draw_elements(mesh_id idx);

			size_t get_vertex_count(mesh_id idx) const { return _data.positions[idx].size(); }
			size_t get_index_count(mesh_id idx) const { return _data.indices[idx].size(); }

			bool has_position_data(mesh_id idx) const { return _data.positions[idx].size() > 0; }
			bool has_normal_data(mesh_id idx) const { return _data.normals[idx].size() > 0; }
			bool has_color_data(mesh_id idx) const { return _data.colors[idx].size() > 0; }
			bool has_uv_data(mesh_id idx) const { return _data.uvs[idx].size() > 0; }
			bool has_tangent_data(mesh_id idx) const { return _data.tangents[idx].size() > 0; }

			void calculate_center(mesh_id idx);

			const vector3& get_center(mesh_id idx) const
			{
				return _data.center[idx];
			}

			const vector3& get_position(mesh_id idx, size_t index) const 
			{ return _data.positions[idx][index]; }

			const std::vector<vector3>& get_positions(mesh_id idx) const
			{ return _data.positions[idx]; }

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

			bounds get_bounds(mesh_id idx)
			{
				return _data.bbox[idx];
			}

			void set_bounds(mesh_id idx, const bounds& bounds)
			{
				_data.bbox[idx] = bounds;
			}

			void set_bounds(mesh_id idx, const vector3& min, const vector3& max)
			{
				_data.bbox[idx] = bounds(min, max);
			}

			const vector3& get_min(mesh_id idx) const
			{
				return _data.bbox[idx].min;
			}

			void set_min(mesh_id idx, const vector3& min)
			{
				_data.bbox[idx].min = min;
			}

			const vector3& get_max(mesh_id idx) const
			{
				return _data.bbox[idx].max;
			}

			void set_max(mesh_id idx, const vector3& max)
			{
				_data.bbox[idx].max = max;
			}

			void set_triangle_mode(mesh_id idx, unsigned mode)
			{
				_data.mode[idx] = mode;
			}

			void set_material(mesh_id idx, material_id mat_id)
			{
				_data.material[idx] = mat_id;
			}

			bool get_state(mesh_id idx) const
			{
				return _data.state[idx];
			}

			unsigned int get_vao(mesh_id idx) const
			{
				return _data.vao[idx];
			}

			unsigned int get_vbo(mesh_id idx) const
			{
				return _data.vbo[idx];
			}

			unsigned int get_ibo(mesh_id idx) const
			{
				return _data.ibo[idx];
			}
	};
}
