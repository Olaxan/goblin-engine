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

		std::shared_ptr<mesh_resource> _mesh;
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

		std::shared_ptr<mesh_resource> get_resource()
		{
			return _mesh;
		}
	};
}
