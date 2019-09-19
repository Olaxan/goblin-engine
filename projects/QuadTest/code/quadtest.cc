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

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec4 Color;

void main()
{
	gl_Position = pos;
	Color = color;
})glsl";

const GLchar* ps = R"glsl(
#version 430

layout(location = 0) in vec4 color;

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

	if (this->window->Open())
	{

		texture = efiilj::TextureResource("res/textures/test.png");
		mesh = efiilj::MeshResource::Cube(1);

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
			printf("[VS SHADER COMPILE ERROR]: %s", buf);
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
			printf("[FS SHADER COMPILE ERROR]: %s", buf);
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

	std::cout << vs << "\n\n === \n\n" << ps;

	while (this->window->IsOpen())
	{

		time++;

		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();

		//float tsin = sinf(float(time) / 30);
		//float tsin2 = sinf(float(time) / 10);
		//float tcos2 = cosf(float(time) / 10);

		//auto trans = efiilj::Matrix4::getTranslation(tsin, std::abs(tcos2 / 3), 0);		// Translation matrix
		//auto rot = efiilj::Matrix4::getRotationZ(float(time) / 30);						// Rotation matrix
		//auto scale = efiilj::Matrix4::getScale(0.75 + tsin2 / 3, 0.75 + tsin2 / 3, 1);	// Scale matrix

		//memcpy(newVert, vertices, 4 * sizeof(efiilj::Vertex));	// Copy original vertex positions to transformation buffer

		//for (int i = 0; i < 4; i++)
		//{
		//	newVert[i].xyzw = (trans * rot * scale) * newVert[i].xyzw;	// Apply transformation matrices to transformation buffer vertices
		//}

		//mesh.UpdateVertexBuffer(newVert);	// Send new vertex data to meshresource
		mesh.Draw(this->program);

		this->window->SwapBuffers();
	}
}

} // namespace Example