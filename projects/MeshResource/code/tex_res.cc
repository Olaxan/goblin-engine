#include "tex_res.h"
#include "stb_image.h"

#include <GL/glew.h>
#include <iostream>

namespace efiilj
{
	texture_resource::texture_resource() : tex_id_(0), height_(0), width_(0), bits_per_pixel_(0)
	{
	}

	texture_resource::texture_resource(const char* path, const bool flip) : tex_id_(0), height_(0), width_(0), bits_per_pixel_(0)
	{
		glGenTextures(1, &tex_id_);
		glBindTexture(GL_TEXTURE_2D, tex_id_);

		stbi_set_flip_vertically_on_load(flip);
		unsigned char* buffer = stbi_load(path, &width_, &height_, &bits_per_pixel_, 4);

		std::cout << "Loaded texture " << tex_id_ << ": " << width_ << "x" << height_ << " / " << bits_per_pixel_ << std::endl;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		unbind();
	}

	texture_resource::texture_resource(const unsigned width, const unsigned height, unsigned* buffer)
	: tex_id_(0), height_(height), width_(width), bits_per_pixel_(32)
	{
		
		glGenTextures(1, &tex_id_);
		glBindTexture(GL_TEXTURE_2D, tex_id_);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_,
			0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		unbind();
	}

	void texture_resource::update(unsigned* buffer) const
	{
		glBindTexture(GL_TEXTURE_2D, tex_id_);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, 
			GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		
		unbind();
	}

	void texture_resource::bind(const unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex_id_);
	}

	void texture_resource::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	texture_resource::~texture_resource()
	{
		std::cout << "Deleting texture resource " << this << std::endl;

		unbind();
		glDeleteTextures(1, &tex_id_);
	}
}
