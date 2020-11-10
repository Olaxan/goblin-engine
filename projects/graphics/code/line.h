#include "node.h"
#include "ray.h"

namespace efiilj
{
	class line : public graphics_node
	{
		private:

			ray line_;

		public:

			line(const ray& line);
			line(const vector4& start, const vector4& end);

			void draw() const override;	
	};
}
