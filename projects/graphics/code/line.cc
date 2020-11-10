#include "line.h"

namespace efiilj
{
	line::line(const ray& line)
		: line_(line) { }

	line::line(const vector4& start, const vector4& end)
		: line_(ray(start, end)) { }

	line::draw() const
	{
		
	}
}
