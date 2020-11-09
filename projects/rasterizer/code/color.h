#pragma once

namespace efiilj
{
	union color
	{
		color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 0xFF)
			: c(0xFFFFFFFF)
		{
			rgba.r = r;
			rgba.g = g;
			rgba.b = b;
			rgba.a = a;
		}

		unsigned int c;

		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} rgba;

		operator unsigned int() const { return c; }
	};
}