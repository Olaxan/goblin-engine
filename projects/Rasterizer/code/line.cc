#include "line.h"


namespace efiilj
{
	line_data::line_data(const vector4& start, const vector4& end)
		: x1(static_cast<int>(start.x())), y1(static_cast<int>(start.y())), x2(static_cast<int>(end.x())), y2(static_cast<int>(end.y()))
	{
		// dx, dy = start-end deltas
		dx = x2 - x1;
		dy = y2 - y1;

		// calculate stepping direction
		step_x = (dx < 0) ? -1 : 1;
		step_y = (dy < 0) ? -1 : 1;

		// convert deltas to absolutes and double them to avoid floating point operations
		dx *= (2 * step_x);
		dy *= (2 * step_y);

		if (dx > dy)
			fraction = dy - (dx >> 1);
		else
			fraction = dx - (dy >> 1);
	}
}
