#include "loader.h"
#include "matrix4.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string.h>

namespace efiilj
{
	bool object_loader::load_from_file(const char* path)
	{
		std::string s;
		std::ifstream file(path);

		std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
		std::vector<vector3> vertices;
		std::vector<vector3> normals;
		std::vector<vector2> uvs;

		std::vector<vertex> packed_vertices;

		if (file.is_open())
		{
			while (std::getline(file, s))
			{
				const char* cs = s.c_str();

				// Vertex normals
				if (strncasecmp(cs, "vn", 2) == 0)
				{
					vector3 norm;

					// Get three floats and add to normals vector
					if (sscanf(&cs[2], "%f %f %f", &norm[0], &norm[1], &norm[2]) == 3)
						normals.push_back(norm);
					else
					{
						std::cout << "\nError when loading OBJ file - failed to parse normal data (" << cs << ")" << std::endl;
						return false;
					}
				}
				// Texture coordinates
				else if (strncasecmp(cs, "vt", 2) == 0)
				{
					vector2 uv;

					// Get two values and add to uvs vector
					if (sscanf(&cs[2], "%f %f", &uv[0], &uv[1]) == 2)
						uvs.push_back(uv);
					else
					{
						std::cout << "\nError when loading OBJ file - failed to parse uv data (" << cs << ")" << std::endl;
						return false;
					}
				}
				// Vertices
				else if (strncasecmp(cs, "v", 1) == 0)
				{
					vector3 vert;

					// Get three floats and add to vertices vector
					if (sscanf(&cs[1], "%f %f %f", &vert[0], &vert[1], &vert[2]) == 3)
						vertices.push_back(vert);
					else
					{
						std::cout << "\nError when loading OBJ file - failed to parse vertex data (" << cs << ")" << std
							::endl;
						return false;
					}
				}
				// Faces
				else if (strncasecmp(cs, "f", 1) == 0)
				{
					unsigned int vertex_index[4], uv_index[4], normal_index[4];

					// Get 12 (quad) or 9 (tri) floats and add to index vectors
					int matches = sscanf(&cs[1], "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
					                     &vertex_index[0], &uv_index[0], &normal_index[0],
					                     &vertex_index[1], &uv_index[1], &normal_index[1],
					                     &vertex_index[2], &uv_index[2], &normal_index[2],
					                     &vertex_index[3], &uv_index[3], &normal_index[3]);

					if (matches >= 9) // Face is valid
					{
						// Add indices to index arrays (vertex, uv, normals)
						
						vertex_indices.push_back(vertex_index[0]);
						vertex_indices.push_back(vertex_index[1]);
						vertex_indices.push_back(vertex_index[2]);

						uv_indices.push_back(uv_index[0]);
						uv_indices.push_back(uv_index[1]);
						uv_indices.push_back(uv_index[2]);

						normal_indices.push_back(normal_index[0]);
						normal_indices.push_back(normal_index[1]);
						normal_indices.push_back(normal_index[2]);

						if (matches == 12)
						{
							// Face is quad - add another tri
							
							vertex_indices.push_back(vertex_index[2]);
							vertex_indices.push_back(vertex_index[3]);
							vertex_indices.push_back(vertex_index[0]);

							uv_indices.push_back(uv_index[3]);
							uv_indices.push_back(uv_index[2]);
							uv_indices.push_back(uv_index[0]);

							normal_indices.push_back(normal_index[2]);
							normal_indices.push_back(normal_index[3]);
							normal_indices.push_back(normal_index[0]);
						}
					}
					else
					{
						std::cout << "\nError when loading OBJ file - failed to parse face data (" << cs << ")" << std::endl;
						return false;
					}
				}
			}

			for (unsigned int i = 0; i < vertex_indices.size(); i++)
			{
				// Convert loose vertices/indices to Vertex objects
				
				unsigned int vertex_index = vertex_indices[i];
				unsigned int uv_index = uv_indices[i];
				unsigned int normal_index = normal_indices[i];

				vector3 xyzw = vertices[vertex_index - 1];
				vector3 norm = normals[normal_index - 1];
				vector2 uv = uvs[uv_index - 1];

				packed_vertices.emplace_back(xyzw, norm, vector4(1, 1, 1, 1), uv);
			}
		}
		else
		{
			std::cout << "\nError when loading OBJ file - could not open file (" << path << ")" << std::endl;
			return false;
		}

		file.close();

		// Optimize vertex buffers and return whether successful
		return find_indices(packed_vertices);
	}

	bool object_loader::find_indices(std::vector<vertex>& in_vertices)
	{
		const int count = in_vertices.size();

		if (count < 3)
			return false;

		index_list_.clear();
		vertex_list_.clear();

		// Map holds vertices and their corresponding indices
		std::map<vertex, unsigned int> vertices;

		for (int i = 0; i < count; i++)
		{
			vertex test = in_vertices[i];

			// Try to find vertices identical to this one in map
			auto it = vertices.find(test);
			if (it != vertices.end())
			{
				// Add vertex index if an identical vertex exists
				index_list_.push_back(it->second);
			}
			else
			{
				// Add vertex and index if no similar vertex is found
				vertex_list_.push_back(test);
				unsigned int index = static_cast<unsigned int>(vertex_list_.size()) - 1;
				index_list_.push_back(index);
				vertices[test] = index;
			}
		}

		return !index_list_.empty();
	}

	object_loader::object_loader(const char* path)
	{
		is_valid_ = load_from_file(path);
	}

	mesh_resource object_loader::get_resource()
	{
		return mesh_resource(vertex_list_.data(), vertex_count(), index_list_.data(), index_count());
	}
}
