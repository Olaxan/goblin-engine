//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"
#include <cstring>
#include <iostream>

const GLchar* vs = R"glsl(
#version 430
layout(location=0) in vec3 pos;
layout(location=1) in vec4 color;
layout(location=0) out vec4 Color;
void main()
{
	gl_Position = vec4(pos, 1);
	Color = color;
})glsl";

const GLchar* ps = R"glsl(
#version 430
layout(location=0) in vec4 color;
out vec4 Color;
void main()
{
	Color = color;
})glsl";

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
QuadTest::QuadTest()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
QuadTest::~QuadTest()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
QuadTest::Open()
{
	App::Open();
	this->window = new Display::Window(1000, 1000);
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
	});

	vertices = new efiilj::Vertex[4]
	{
		efiilj::Vertex(-0.25f, -0.25f, -1, 0, 0, 1, 1),
		efiilj::Vertex(0.25f, -0.25f, -1, 0, 1, 0, 1),
		efiilj::Vertex(0.25f, 0.25f, -1, 1, 0, 0, 1),
		efiilj::Vertex(-0.25f, 0.25f, -1, 1, 1, 1, 1)
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	if (this->window->Open())
	{
		mesh = efiilj::MeshResource(vertices, 4, indices, 6);

		// set clear color to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = static_cast<GLint>(std::strlen(vs));
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = static_cast<GLint>(std::strlen(ps));
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
QuadTest::Run()
{
	while (this->window->IsOpen())
	{
		time++;

		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();

		float tsin = sinf(float(time) / 30);

		auto trans = efiilj::Matrix4::getTranslation(tsin, cosf(tsin / 2) / 5, 0);
		auto rot = efiilj::Matrix4::getRotationZ(float(time) / 30);
		auto scale = efiilj::Matrix4::getScale(tsin, tsin, tsin);

		efiilj::Vertex newVert[4];
		memcpy(newVert, vertices, 4 * sizeof(efiilj::Vertex));

		for (int i = 0; i < 4; i++)
		{
			newVert[i].xyzw = (trans * rot * scale) * newVert[i].xyzw;
		}

		mesh.UpdateVertexBuffer(newVert);
		mesh.Draw(this->program);

		this->window->SwapBuffers();
	}
}

} // namespace Example