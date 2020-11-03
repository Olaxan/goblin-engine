#pragma once

#include "vertex.h"
#include "mesh_res.h"

#include <vector>

namespace efiilj
{
	/**
	 * \brief Class which aids in the loading and parsing of Wavefront OBJ files.
	 */
	class object_loader
	{
	private:

		std::vector<vertex> vertex_list_;
		std::vector<unsigned> index_list_;
		bool is_valid_;

		bool load_from_file(const char* path);
		bool find_indices(std::vector<vertex>& in_vertices);

	public:

		/**
		 * \brief Creates a new Object Loader instance.
		 * \param path The path to the specific OBJ file that should be loaded
		 */
		explicit object_loader(const char* path);

		/**
		 * \brief Returns whether or not the loader contains valid data.
		 * \return True if object loaded successfully, false otherwise
		 */
		bool is_valid() const { return is_valid_; }

		std::vector<vertex> get_vertices() const { return vertex_list_; }
		std::vector<unsigned> get_indices() const { return index_list_; }
		
		int vertex_count() const { return vertex_list_.size(); }
		int index_count() const { return index_list_.size(); }

		std::shared_ptr<mesh_resource> get_resource();
	};
}
