#pragma once

#include "mesh_res.h"

namespace efiilj
{
	class bbox : public mesh_resource
	{
		private:

		public:

			bbox(const vector3& min, const vector3& max);

			void draw_elements() const override;
	};
}
