#pragma once

namespace efiilj
{
	/// <summary>
	/// Class to create a texture on the GPU, as well as to hold buffer handles which enables binding.
	/// </summary>
	class texture_resource
	{
	private:

		/// <summary>
		/// Texture object handle.
		/// </summary>
		unsigned int tex_id_;

		int height_, width_, bits_per_pixel_;
	public:

		/// <summary>
		/// Creates an empty TextureResource object.
		/// </summary>
		texture_resource();

		/// <summary>
		/// Creates a new TextureResource object from the image resource at the specified file path.
		/// </summary>
		/// <param name="path">The path to the image resource</param>
		/// <param name="flip">Whether the texture should be flipped vertically on load, as some formats require</param>
		texture_resource(const char* path, bool flip);

		/// <summary>
		/// Binds the texture to the specified texture slot, to prepare OpenGL for utilizing this texture.
		/// </summary>
		/// <param name="slot">The GPU texture slot to use (typically 0 - 8/16)</param>
		void bind(unsigned int slot = 0) const;

		/// <summary>
		/// Unbinds the resource from the OpenGL state machine.
		/// </summary>
		static void unbind();

		~texture_resource();
	};
}
