#pragma once
#include "matrix4.h"


namespace efiilj
{
	struct point_data
	{
		int x, y;
	};
	
	struct line_data
	{	
		int x1, y1, x2, y2, dx, dy, step_x, step_y, fraction;
		
		line_data(const vector4& start, const vector4& end);
	};	
}