#pragma once

#include "vertex.h"
#include "mesh_srv.h"

#include <vector>
#include <filesystem>

namespace efiilj
{
	/**
	 * \brief Class which aids in the loading and parsing of Wavefront OBJ files.
	 */
	class object_loader
	{
	private:

		mesh_id _mesh;
		bool _is_valid;

		bool load_from_file(const std::filesystem::path& uri);
		bool find_indices(std::vector<vertex>& in_vertices);

		std::shared_ptr<mesh_server> _meshes;

	public:

		object_loader(const std::filesystem::path& uri, std::shared_ptr<mesh_server> meshes);
		~object_loader();

		bool is_valid() const { return _is_valid; }
		mesh_id get_mesh() const { return _mesh; }

	};
}
