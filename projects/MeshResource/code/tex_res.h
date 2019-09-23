#pragma once

namespace efiilj
{
	class TextureResource
	{
	private:
		unsigned int tex_id;
		int height, width, bits_per_pixel;
	public:
		TextureResource();
		TextureResource(const char* path);

		void Bind(unsigned int slot = 0);
		void Unbind();

		~TextureResource();
	};
}