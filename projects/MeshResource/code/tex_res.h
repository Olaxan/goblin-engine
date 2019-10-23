#pragma once

namespace efiilj
{
	/**
	 * \brief Class to create a texture on the GPU, as well as to hold buffer handles which enables binding.
	 */
	class texture_resource
	{
	private:

		/**
		 * \brief Texture object handle
		 */
		unsigned int tex_id_;

		int height_, width_, bits_per_pixel_;
		
	public:

		/** 
		 * \brief Creates an empty TextureResource instance.
		 */ 
		texture_resource();

		/**
		 * \brief Creates a new TextureResource object from the image resource at the specified file path.
		 * \param path The path to the image resource
		 * \param flip Whether the texture should be flipped vertically on load, as required by some formats
		 */
		texture_resource(const char* path, bool flip);

		texture_resource(unsigned width, unsigned height, unsigned* buffer);

		void update(unsigned* buffer) const;
		
		/**
		 * \brief Binds the texture to the specified texture slot, to prepare OpenGL for utilizing this texture.
		 * \param slot The GPU texture slot to use (typically 0 - 8/16)
		 */
		void bind(unsigned int slot = 0) const;

		/**
		 * \brief Unbinds all textures from the GPU.
		 */
		static void unbind();

		~texture_resource();
	};
}
