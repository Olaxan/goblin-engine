#include "line.h"

#include <cmath>


namespace efiilj
{
	line_data::line_data(const vector4& start, const vector4& end)
		: x1(static_cast<int>(start.x())), y1(static_cast<int>(start.y())), x2(static_cast<int>(end.x())), y2(static_cast<int>(end.y())),
		  dx(std::abs(x2 - x1) << 1), dy(std::abs(y2 - y1) << 1), step_x(x2 - x1 < 0 ? -1 : 1), step_y(y2 - y1 < 0 ? -1 : 1),
		  horizontal(dx > dy), curr_x(x1), curr_y(y1), fraction(horizontal ? dy - (dx >> 1) : dx - (dy >> 1))
	
	{ }

	void line_data::reset()
	{
		curr_x = x1;
		curr_y = y1;
		fraction = horizontal ? dy - (dx >> 1) : dx - (dy >> 1);
	}
}
