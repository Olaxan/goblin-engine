#include "line.h"

#include <GL/glew.h>

namespace efiilj
{
	line::line(const ray& line, float length, float width)
		: line_(line), width_(width)
	{ 
		init_array_object();

		vector4 points[] = 
		{
			line.start,
			line.start + line.dir * length
		};
		
		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ray), &points, GL_STATIC_DRAW); 
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	line::line(const vector4& start, const vector4& end, float width)
		: line(ray(start, (end - start).norm()), (end - start).length(), width) 
	{}

	void line::draw_elements() const
	{
		glLineWidth(width_);
		glDrawArrays(GL_LINES, 0, 2);
		glLineWidth(1.0f);
	}
}
