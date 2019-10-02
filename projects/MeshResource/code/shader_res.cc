#include "shader_res.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace efiilj
{
	shader_resource::shader_resource() : program_id_(0)
	{
	}

	shader_resource::shader_resource(const char* vertex, const char* fragment) : program_id_(0)
	{
		program_id_ = create_program(vertex, fragment);
	}

	shader_resource::shader_resource(const std::string& vertex, const std::string& fragment) : shader_resource(
		vertex.c_str(), fragment.c_str())
	{
	}

	std::string shader_resource::load_shader(const char* file_path)
	{
		std::string s;
		std::stringstream ss;
		std::ifstream file(file_path);

		if (file.is_open())
		{
			while (std::getline(file, s))
				ss << s << "\n";
		}

		return ss.str();
	}

	std::string shader_resource::load_shader(const std::string& file_path)
	{
		return load_shader(file_path.c_str());
	}

	unsigned int shader_resource::compile_shader(const unsigned int type, const char* source)
	{
		const unsigned int id = glCreateShader(type);

		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);
		debug_shader(id, type_shader, GL_COMPILE_STATUS, std::cout,
		            (type == GL_VERTEX_SHADER ? "Vertex compilation failed!" : "Fragment compilation failed!"));

		return id;
	}

	unsigned int shader_resource::create_program(const char* vertex, const char* fragment)
	{
		const unsigned int program = glCreateProgram();
		const unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex);
		const unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment);

		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glLinkProgram(program);
		debug_shader(program, type_program, GL_LINK_STATUS, std::cout, "Program linking failed!");

		glValidateProgram(program);
		debug_shader(program, type_program, GL_VALIDATE_STATUS, std::cout, "Program validation failed!");

		glDetachShader(program, vs);
		glDetachShader(program, fs);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	bool shader_resource::debug_shader(const unsigned int id, const shader_debug_type type, const unsigned int status, std::ostream& stream,
	                                 const char* header)
	{
		int result = 0;
		int log_size = 0;
		char* message = "";

		switch (type)
		{
		case type_shader:
			glGetShaderiv(id, status, &result);
			if (result == GL_FALSE)
			{
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_size);
				message = static_cast<char*>(nullptr);
				glGetShaderInfoLog(id, log_size, nullptr, message);
			}
			break;
		case type_program:
			glGetProgramiv(id, status, &result);
			if (result == GL_FALSE)
			{
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
				message = static_cast<char*>(nullptr);
				glGetProgramInfoLog(id, log_size, nullptr, message);
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

	void shader_resource::use() const
	{
		glUseProgram(program_id_);
	}

	void shader_resource::drop()
	{
		glUseProgram(0);
	}

	int shader_resource::find_uniform_location(const char* name)
	{
		int uniform;
		const auto it = locations_.find(name);

		if (it == locations_.end())
		{
			uniform = glGetUniformLocation(program_id_, name);
			if (uniform != -1)
				locations_[name] = uniform;
		}
		else
			uniform = it->second;

		return uniform;
	}

	bool shader_resource::set_uniform1_i(const char* name, const int val)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform1i(uniform, val);
		return true;
	}

	bool shader_resource::set_uniform_vector4_fv(const char* name, const Vector4& vec)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform4fv(uniform, 1, &vec.at(0));
		return true;
	}

	bool shader_resource::set_uniform_matrix4_fv(const char* name, const Matrix4& mat)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniformMatrix4fv(uniform, 1, GL_TRUE, &mat.at(0));
		return true;
	}

	shader_resource::~shader_resource()
	{
		glDeleteProgram(program_id_);
	}
}
