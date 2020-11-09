#pragma once

#include "matrix4.h"
#include "color.h"

namespace efiilj
{

	/**
	 * \brief A class to load and represent a texture in byte format.
	 */
	class texture_data
	{
	private:
		unsigned char* texture_;
		int width_ {};
		int height_ {};
		int bits_per_pixel_ {};

	public:
		/**
		 * \brief Creates a new texture instance.
		 * \param path The path to the texture file
		 * \param comp Set channel count to a specific value
		 */
		explicit texture_data(const char* path, int comp = 0);
		~texture_data();

		//TODO: Implement maybe
		texture_data(texture_data& other);
		texture_data(texture_data&& other); 

		/**
		 * \brief Get the width of the texture.
		 * \return The texture width in pixels
		 */
		int width() const { return width_; }

		/**
		 * \brief Get the height of the texture.
		 * \return The texture height in pixels
		 */
		int height() const { return height_; }
		
		/**
		 * \brief Gets the number of bits per pixel.
		 * \return The BPP-value of the texture
		 */
		int bits_per_pixel() const { return bits_per_pixel_; }
		
		/**
		 * \brief Returns a pointer to the texture data array.
		 * \return A pointer to the first color channel in the texture
		 */
		unsigned char* texture() const { return texture_; }

		/**
		 * \brief Samples a specific point on the texture and returns the color.
		 * \param uv The UV point to sample
		 * \return The color of the point, or black upon error
		 */
		vector4 get_pixel(const vector2& uv) const;
		
	};
}
