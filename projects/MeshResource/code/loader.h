#pragma once

#include "vertex.h"
#include "mesh_res.h"

#include <vector>

namespace efiilj
{
	class ObjectLoader
	{
	private:

		std::vector<Vertex> vertexList;
		std::vector<unsigned int> indexList;
		bool is_valid;

		bool LoadFromFile(const char* path);

	public:

		ObjectLoader(const char* path);

		bool isValid() { return is_valid; }

		int vertexCount() { return vertexList.size(); }
		int indexCount() { return indexList.size(); }

		MeshResource GetResource();
	};
}