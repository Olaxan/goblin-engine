//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
namespace example
{
	example_app::example_app()
		= default;

	example_app::~example_app()
		= default;


	bool example_app::open()
	{
		app::open();
		this->window_ = new Display::Window;
		window_->SetKeyPressFunction([this](int32, int32, int32, int32)
		{
			this->window_->Close();
		});

		GLfloat buf[] =
		{
			-0.5f,	-0.5f,	-1,			// pos 0
			1,		0,		0,		1,	// color 0
			0,		0.5f,	-1,			// pos 1
			0,		1,		0,		1,	// color 0
			0.5f,	-0.5f,	-1,			// pos 2
			0,		0,		1,		1	// color 0
		};

		if (this->window_->Open())
		{
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			// setup vertex shader
			this->vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
			GLint length = static_cast<GLint>(std::strlen(vs));
			glShaderSource(this->vertex_shader_, 1, &vs, &length);
			glCompileShader(this->vertex_shader_);

			// get error log
			GLint shaderLogSize;
			glGetShaderiv(this->vertex_shader_, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0)
			{
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->vertex_shader_, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}

			// setup pixel shader
			this->pixel_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
			length = static_cast<GLint>(std::strlen(ps));
			glShaderSource(this->pixel_shader_, 1, &ps, &length);
			glCompileShader(this->pixel_shader_);

			// get error log
			shaderLogSize;
			glGetShaderiv(this->pixel_shader_, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0)
			{
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->pixel_shader_, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}

			// create a program object
			this->program_ = glCreateProgram();
			glAttachShader(this->program_, this->vertex_shader_);
			glAttachShader(this->program_, this->pixel_shader_);
			glLinkProgram(this->program_);
			glGetProgramiv(this->program_, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0)
			{
				GLchar* buf = new GLchar[shaderLogSize];
				glGetProgramInfoLog(this->program_, shaderLogSize, NULL, buf);
				printf("[PROGRAM LINK ERROR]: %s", buf);
				delete[] buf;
			}

			// setup vbo
			glGenBuffers(1, &this->triangle_);
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle_);
			glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	example_app::run()
	{
		while (this->window_->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT);
			this->window_->Update();

			// do stuff
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle_);
			glUseProgram(this->program_);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			this->window_->SwapBuffers();
		}
	}
}