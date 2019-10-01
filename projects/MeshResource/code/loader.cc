#include "loader.h"
#include "matrix4.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

namespace efiilj
{
	bool ObjectLoader::LoadFromFile(const char* path)
	{
		std::string s;
		std::ifstream file(path);

		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;

		std::vector<Vertex> packedVertices;

		if (file.is_open())
		{
			while (std::getline(file, s))
			{

				const char* cs = s.c_str();

				if (strnicmp(cs, "vn", 2) == 0)
				{
					Vector3 norm;
					if (std::sscanf(&cs[2], "%f %f %f", &norm[0], &norm[1], &norm[2]) == 3)
						normals.push_back(norm);
					else
					{
						std::cout << "\nError when loading OBJ file - failed to parse normal data (" << cs << ")" << std::endl;
						return false;
					}
				}
				else if (strnicmp(cs, "vt", 2) == 0)
				{
					Vector2 uv;
					if (std::sscanf(&cs[2], "%f %f", &uv[0], &uv[1]) == 2)
						uvs.push_back(uv);
					else
					{
						std::cout << "\nError when loading OBJ file - failed to parse uv data (" << cs << ")" << std::endl;
						return false;
					}
				}
				else if (strnicmp(cs, "v", 1) == 0)
				{
					Vector3 vert;
					if (sscanf(&cs[1], "%f %f %f", &vert[0], &vert[1], &vert[2]) == 3)
						vertices.push_back(vert);
					else
					{
						std::cout << "\nError when loading OBJ file - failed to parse vertex data (" << cs << ")" << std::endl;
						return false;
					}
				}
				else if (strnicmp(cs, "f", 1) == 0)
				{
					unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
					int matches = sscanf(&cs[1], "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
						&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2],
						&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

					if (matches == 9 || matches == 12)
					{
						vertexIndices.push_back(vertexIndex[0]);
						vertexIndices.push_back(vertexIndex[1]);
						vertexIndices.push_back(vertexIndex[2]);

						uvIndices.push_back(uvIndex[0]);
						uvIndices.push_back(uvIndex[1]);
						uvIndices.push_back(uvIndex[2]);

						normalIndices.push_back(normalIndex[0]);
						normalIndices.push_back(normalIndex[1]);
						normalIndices.push_back(normalIndex[2]);
					}
					else
					{
						std::cout << "\nError when loading OBJ file - failed to parse face data (" << cs << ")" << std::endl;
						return false;
					}

					if (matches == 12)
					{
						std::cout << "Triangulating quadrilateral (" << s << ")\n";

						vertexIndices.push_back(vertexIndex[2]);
						vertexIndices.push_back(vertexIndex[3]);
						vertexIndices.push_back(vertexIndex[0]);

						uvIndices.push_back(uvIndex[3]);
						uvIndices.push_back(uvIndex[2]);
						uvIndices.push_back(uvIndex[0]);

						normalIndices.push_back(normalIndex[2]);
						normalIndices.push_back(normalIndex[3]);
						normalIndices.push_back(normalIndex[0]);
					}
				}
				else
				{
					std::cout << "Skipped line " << s << "\n";
				}
			}

			for (unsigned int i = 0; i < vertexIndices.size(); i++)
			{
				unsigned int vertexIndex = vertexIndices[i];
				unsigned int uvIndex = uvIndices[i];
				unsigned int normalIndex = normalIndices[i];

				Vector3 xyzw = vertices[vertexIndex - 1];
				Vector3 norm = normals[normalIndex - 1];
				Vector2 uv = uvs[uvIndex - 1];

				packedVertices.push_back(Vertex(xyzw, norm, Vector4(1, 1, 1, 1), uv));
			}
		}
		else
		{
			std::cout << "\nError when loading OBJ file - could not open file (" << path << ")" << std::endl;
			return false;
		}
		
		file.close();
		return FindIndices(packedVertices);
	}

	bool ObjectLoader::FindIndices(std::vector<Vertex>& in_vertices)
	{
		int count = in_vertices.size();

		if (count < 3)
			return false;

		indexList.clear();
		vertexList.clear();

		Vertex test;
		std::map<Vertex, unsigned int> vertices;
		std::map<Vertex, unsigned int>::iterator it;

		for (int i = 0; i < count; i++)
		{
			test = in_vertices[i];

			auto it = vertices.find(test);
			if (it != vertices.end())
			{
				indexList.push_back(it->second);
			}
			else
			{
				vertexList.push_back(test);
				unsigned int index = (unsigned int)vertexList.size() - 1;
				indexList.push_back(index);
				vertices[test] = index;
			}
		}

		return indexList.size() > 0;
	}

	ObjectLoader::ObjectLoader(const char* path)
	{
		is_valid = LoadFromFile(path);
	}

	MeshResource ObjectLoader::GetResource()
	{
		return MeshResource(vertexList.data(), vertexCount(), indexList.data(), indexCount());
	}
}

