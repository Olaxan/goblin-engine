#pragma once

#include "vertex.h"
#include "mesh_res.h"

#include <vector>

namespace efiilj
{
	class object_loader
	{
	private:

		std::vector<vertex> vertex_list_;
		std::vector<unsigned int> index_list_;
		bool is_valid_;

		bool load_from_file(const char* path);
		bool find_indices(std::vector<vertex>& in_vertices);

	public:

		explicit object_loader(const char* path);

		bool is_valid() const { return is_valid_; }

		int vertex_count() const { return vertex_list_.size(); }
		int index_count() const { return index_list_.size(); }

		mesh_resource get_resource();
	};
}
