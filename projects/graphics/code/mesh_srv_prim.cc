#include "mesh_srv.h"

namespace efiilj
{

	void mesh_server::create_cube(mesh_id idx) //NOLINT
	{

	}

	void mesh_server::create_bbox(mesh_id idx)
	{
		// if buffered, do nothing
		if (_data.state[idx])
			return;

		std::vector<unsigned int> indices = {
			0, 1,
			0, 3,
			0, 4,
			2, 1,
			2, 3,
			2, 7,
			6, 3,
			6, 4,
			6, 7,
			5, 1,
			5, 4,
			5, 7
		};

		set_indices(idx, indices);

		vector3 size = vector3(1, 1, 1);

		bounds b(size * -0.5f, size * 0.5f);

		std::vector<vector3> positions = {
			vector3(b.max.x, b.min.y, b.min.z),
			vector3(b.max.x, b.max.y, b.min.z),
			vector3(b.min.x, b.max.y, b.min.z),
			vector3(b.min.x, b.min.y, b.min.z),
			vector3(b.max.x, b.min.y, b.max.z),
			vector3(b.max.x, b.max.y, b.max.z),
			vector3(b.min.x, b.min.y, b.max.z),
			vector3(b.min.x, b.max.y, b.max.z)
		};

		set_positions(idx, positions);
	}

	mesh_id mesh_server::create_primitive(primitive type)
	{
		mesh_id idx = create();

		switch (type)
		{
			case primitive::cube:
				create_cube(idx);
				break;

			case primitive::bbox:
				create_bbox(idx);
				break;

			default:
				fprintf(stderr, "Invalid primitive!\n");
				break;
		}

		return idx;
	};
}
