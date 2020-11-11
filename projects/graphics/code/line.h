#pragma once

#include "mesh_res.h"
#include "ray.h"

namespace efiilj
{
	class line : public mesh_resource
	{
		private:

			ray line_;
			float width_;

		public:

			line(const ray& line);
			line(const vector4& start, const vector4& end);

			void set_width(float w) { width_ = w; }

			void draw_elements() const override;
	};
}
