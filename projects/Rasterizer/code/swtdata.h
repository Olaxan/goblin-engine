#pragma once

#include "matrix4.h"
#include "color.h"

namespace efiilj
{

	class texture_data
	{
	private:
		unsigned char* texture_;
		int width_ {};
		int height_ {};
		int bits_per_pixel_ {};

	public:
		explicit texture_data(const char* path, int comp = 0);
		~texture_data();

		//TODO: Implement maybe
		texture_data(texture_data& other);
		texture_data(texture_data&& other); 

		int width() const { return width_; }
		int height() const { return height_; }
		int bits_per_pixel() const { return bits_per_pixel_; }
		unsigned char* texture() const { return texture_; }

		vector4 get_pixel(const vector2& uv) const;
		
	};
}
