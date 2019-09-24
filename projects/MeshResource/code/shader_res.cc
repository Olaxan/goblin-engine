#include "shader_res.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace efiilj
{
	ShaderResource::ShaderResource() : program_id(0)
	{
	}

	ShaderResource::ShaderResource(const char* vertex, const char* fragment) : program_id(0)
	{
		program_id = CreateProgram(vertex, fragment);
	}

	std::string ShaderResource::LoadShader(const char* filePath)
	{
		std::string s;
		std::stringstream ss;
		std::ifstream file(filePath);

		if (file.is_open())
		{
			while (std::getline(file, s))
				ss << s << "\n";
		}

		return ss.str();
	}

	unsigned int ShaderResource::CompileShader(unsigned int type, const char* source)
	{
		unsigned int id = glCreateShader(type);

		glShaderSource(id, 1, &source, NULL);
		glCompileShader(id);
		DebugShader(id, ShaderDebugType::SHADER, GL_COMPILE_STATUS, std::cout, (type == GL_VERTEX_SHADER ? "Vertex compilation failed!" : "Fragment compilation failed!"));

		return id;
	}

	unsigned int ShaderResource::CreateProgram(const char* vertex, const char* fragment)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment);

		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glLinkProgram(program);
		DebugShader(program, ShaderDebugType::PROGRAM, GL_LINK_STATUS, std::cout, "Program linking failed!");

		glValidateProgram(program);
		DebugShader(program, ShaderDebugType::PROGRAM, GL_VALIDATE_STATUS, std::cout, "Program validation failed!");

		glDetachShader(program, vs);
		glDetachShader(program, fs);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	bool ShaderResource::DebugShader(unsigned int id, ShaderDebugType type, unsigned int status, std::ostream& stream, const char* header)
	{
		int result = 0;
		int logSize = 0;
		char* message = "";

		switch (type)
		{
		case ShaderResource::SHADER:
			glGetShaderiv(id, status, &result);
			if (result == GL_FALSE)
			{
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);
				message = (char*)_malloca(logSize * sizeof(char));
				glGetShaderInfoLog(id, logSize, NULL, message);
			}
			break;
		case ShaderResource::PROGRAM:
			glGetProgramiv(id, status, &result);
			if (result == GL_FALSE)
			{
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logSize);
				message = (char*)_malloca(logSize * sizeof(char));
				glGetProgramInfoLog(id, logSize, NULL, message);
			}
			break;
		default:
			message = "Unknown debug type!";
			break;
		}

		if (!result)
			stream << "DEBUG: " << header << "\n" << message << std::endl;

		return result;
	}

	void ShaderResource::Use()
	{
		glUseProgram(program_id);
	}

	void ShaderResource::Drop()
	{
		glUseProgram(0);
	}

	int ShaderResource::FindUniformLocation(const char* name)
	{
		int uniform;
		auto it = locations.find(name);

		if (it == locations.end())
		{
			uniform = glGetUniformLocation(program_id, name);
			if (uniform != -1)
				locations[name] = uniform;
		}
		else
			uniform = it->second;

		return uniform;
	}

	bool ShaderResource::SetUniform1i(const char* name, int val)
	{
		int uniform = FindUniformLocation(name);

		if (uniform == -1)
			return false;

		glUniform1i(uniform, val);
		return true;
	}

	bool ShaderResource::SetUniformMatrix4fv(const char* name, Matrix4& mat)
	{
		int uniform = FindUniformLocation(name);

		if (uniform == -1)
			return false;

		glUniformMatrix4fv(uniform, 1, GL_TRUE, &mat(0));
		return true;
	}

	ShaderResource::~ShaderResource()
	{
		glDeleteProgram(program_id);
	}
}

