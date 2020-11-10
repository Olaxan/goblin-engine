#include "rect.h"

#include <GL/glew.h>

namespace efiilj
{
	rect::rect()
	{

		float quad[] = {
			-1.0f,	1.0f, 0.0f, 1.0f,  
			-1.0f, -1.0f, 0.0f, 1.0f, 
			 1.0f,	1.0f, 0.0f, 1.0f, 
			 1.0f, -1.0f, 0.0f, 1.0f
			};	

		init_array_object();

		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);		
	}

	void rect::draw_elements() const
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}
