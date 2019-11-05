#include "swtdata.h"
#include "color.h"

#include <stb_image.h>
#include <cstring>

namespace efiilj
{
	texture_data::texture_data(const char* path, const int comp)
	{
		texture_ = stbi_load(path, &width_, &height_, &bits_per_pixel_, comp);
	}

	texture_data::~texture_data()
	{
		stbi_image_free(texture_);
	}

	vector4 texture_data::get_pixel(const vector2& uv) const
	{
		//TODO: Check precision of point-on-line algorithm to avoid this.
		if (uv.x() < 0 || uv.y() < 0)
			return vector4(0,0,0,0);
		
		const int tex_x = static_cast<int>(static_cast<float>(width_)  * uv.x()) % width_;
		const int tex_y = static_cast<int>(static_cast<float>(height_)* uv.y()) % height_;
		const unsigned char* pos = &texture_[tex_x + width_ * tex_y];

		return vector4(pos[0], pos[1], pos[2], bits_per_pixel_ == 4 ? pos[3] : 255);
	}
}
