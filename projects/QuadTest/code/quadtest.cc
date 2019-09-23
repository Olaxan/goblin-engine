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

layout(location = 0) out vec2 Uv;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * pos;
	Uv = uv;
})glsl";

const GLchar* ps = R"glsl(
#version 430

layout(location = 0) in vec2 uv;

uniform sampler2D u_sampler;

out vec4 Color;

void main()
{
	Color = texture(u_sampler, uv);
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
	this->window = new Display::Window(720, 720);
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
	});

	t_start = std::chrono::high_resolution_clock::now();

	if (this->window->Open())
	{

		texture = efiilj::TextureResource("C:/Users/efiilj-7-local/Documents/Source/ltu-lab-s0006e_env/bin/res/textures/test2.png");
		mesh = efiilj::MeshResource::Cube(1);

		//enable face culling
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

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

	std::cout << "Compiled successfully!\n\nVERTEX SHADER:\n" << vs << "\n\nFRAGMENT SHADER:\n" << ps << "\n\n === \n\n";

	float fov = nvgDegToRad(75);

	while (this->window->IsOpen())
	{

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		//efiilj::Matrix4 model = efiilj::Matrix4::getTranslation(0.0f, 0.0f, 0.0f);
		efiilj::Matrix4 view = efiilj::Matrix4::getLookat(efiilj::Vector3(2 * sinf(time), 1, 2 * cosf(time)), efiilj::Vector3(0.0f, 0.0f, 0.0f), efiilj::Vector3(0.0f, 1.0f, 0.0f));
		efiilj::Matrix4 perspective = efiilj::Matrix4::getPerspective(fov, 1.0f, 0.1f, 100.0f);

		efiilj::Matrix4 mvp = (perspective * view);

		glUseProgram(program);

		GLint uniMVP = glGetUniformLocation(program, "mvp");
		glUniformMatrix4fv(uniMVP, 1, GL_TRUE, &mvp(0));

		GLint uniSampler = glGetUniformLocation(program, "u_sampler");
		glUniform1i(uniSampler, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();

		texture.Bind();
		mesh.Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

		this->window->SwapBuffers();
	}
}

} // namespace Example