#include "shader_res.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace efiilj
{
	shader_resource::shader_resource()
	: shader_id_(0), shader_state_(GL_FALSE) { }

	shader_resource::shader_resource(unsigned type, const char* path) 
		: shader_id_(0), shader_state_(GL_FALSE)
	{
		std::string source = load_shader(path);
		shader_state_ = compile_shader(type, source.c_str());
	}

	shader_resource::shader_resource(unsigned type, const std::string& path)
	: shader_resource(type, path.c_str()) { }

	std::string shader_resource::load_shader(const char* file_path)
	{
		std::string s;
		std::stringstream ss;
		std::ifstream file(file_path);

		if (file.is_open())
		{
			while (std::getline(file, s))
				ss << s << "\n";
			
			file.close();
		}
		
		return ss.str();
	}

	std::string shader_resource::load_shader(const std::string& file_path)
	{
		return load_shader(file_path.c_str());
	}

	int shader_resource::compile_shader(const unsigned type, const char* source)
	{
		int result;
		shader_id_ = glCreateShader(type);

		glShaderSource(shader_id_, 1, &source, nullptr);
		glCompileShader(shader_id_);
		glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &result);
		debug_shader(shader_id_, type_shader, GL_COMPILE_STATUS, std::cout,
		            (type == GL_VERTEX_SHADER ? "Vertex compilation failed!" : "Fragment compilation failed!"));

		return result;
	}

	bool shader_resource::attach(unsigned program)
	{ 
		glAttachShader(program, shader_id_);
		return shader_state_ == GL_TRUE;
	}
        
	void shader_resource::detach(unsigned program)
	{ 
		glDetachShader(program, shader_id_); 
	}

	void shader_resource::free()
	{
		glDeleteShader(shader_id_);
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
				message = static_cast<char*>(alloca(log_size * sizeof(char)));
				glGetShaderInfoLog(id, log_size, nullptr, message);
			}
			break;
		case type_program:
			glGetProgramiv(id, status, &result);
			if (result == GL_FALSE)
			{
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
				message = static_cast<char*>(alloca(log_size * sizeof(char)));
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

	shader_resource::~shader_resource()
	{
		//glDeleteProgram(program_id_);
	}
}
