#include "swtdata.h"
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
}
