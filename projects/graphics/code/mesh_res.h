#pragma once

#include <memory>
#include <vector>

#include "vertex.h"
#include "bounds.h"

namespace efiilj
{
	
	/**
	 * \brief Class to create a mesh on the GPU, as well as to hold buffer handles which enables binding.
	 */
	class mesh_resource
	{
		protected:

		unsigned _vbo, _ibo, _vao, _type, _usage;

		bounds _bounds;

		std::vector<vector3> _positions;
		std::vector<vector3> _normals;
		std::vector<vector2> _uvs;
		std::vector<vector4> _tangents;
		std::vector<unsigned> _indices;

		void init_vertex_buffer();
		void init_index_buffer();
		void init_array_object();

		void set_bounds(const vector3& min, const vector3& max) { _bounds = bounds(min,  max); }
		void set_bounds(const bounds& bounds) { _bounds = bounds; }

	public:

		mesh_resource();
		mesh_resource(unsigned type, unsigned vao, unsigned vbo, unsigned ibo=0, int vertex_count=-1, int index_count=-1);

		int material_index;

		unsigned vao() const { return this->_vao; }
		unsigned vbo() const { return this->_vbo; }
		unsigned ibo() const { return this->_ibo; }

		unsigned usage() const { return this->_usage; }

		size_t vertex_count() const { return _positions.size(); }
		size_t index_count() const { return _indices.size(); }

		bool has_position_data() const { return _positions.size() > 0; }
		bool has_normal_data() const { return _normals.size() > 0; }
		bool has_uv_data() const { return _uvs.size() > 0; }
		bool has_tangent_data() const { return _tangents.size() > 0; }

		const vector3& get_position(size_t index) const { return _positions[index]; }
		const vector3& get_indexed_position(size_t index) const { return get_position(_indices[index]); }

		const vector3& get_normal(size_t index) const { return _normals[index]; }
		const vector3& get_indexed_normal(size_t index) const { return get_normal(_indices[index]); }

		const vector2& get_uv(size_t index) const { return _uvs[index]; }
		const vector2& get_indexed_uv(size_t index) const { return get_uv(_indices[index]); }

		const vector4& get_tangent(size_t index) const { return _tangents[index]; }
		const vector4& get_indexed_tangent(size_t index) const { return get_tangent(_indices[index]); }

		void finalize();

		void bind() const;
		void unbind();

		void clear();

		virtual void draw_elements() const;

		void buffer(unsigned target, size_t size, void* data, unsigned usage);

		const bounds& get_bounds() { return _bounds; }
		bounds get_bounds(const matrix4& mat) { return _bounds.get_transformed_bounds(mat); }

		~mesh_resource();

		friend class gltf_model_loader;
		friend class object_loader;
	};
}
