#include "def_rend.h"

#include "GL/glew.h"

#include <cassert>

namespace efiilj
{
	deferred_renderer::deferred_renderer
		(
			int width, 
			int height, 
			std::shared_ptr<shader_program> geometry, 
			std::shared_ptr<shader_program> lighting
		)
		: width(width), height(height), geometry_(std::move(geometry)), lighting_(std::move(lighting))
	{
		// Setup gbuffer
		glGenFramebuffers(1, &gbo);
		glBindFramebuffer(GL_FRAMEBUFFER, gbo);

		gen_buffer(&pos, GL_FLOAT);
		gen_buffer(&norm, GL_FLOAT);
		gen_buffer(&cspec, GL_UNSIGNED_BYTE);

		attachments[0] = GL_COLOR_ATTACHMENT0;
		attachments[1] = GL_COLOR_ATTACHMENT1;
		attachments[2] = GL_COLOR_ATTACHMENT3;

		glDrawBuffers(3, attachments);

		// Setup render buffer + depth
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// Check if framebuffer is ready
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	void deferred_renderer::gen_buffer(unsigned* handle, unsigned type)
	{
		glGenTextures(1, handle);
		glBindTexture(GL_TEXTURE_2D, *handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *handle, 0);
	}	
}
