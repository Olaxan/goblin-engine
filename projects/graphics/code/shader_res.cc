#include "shader_res.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _MSC_VER 
#define alloca _malloca
#endif

namespace efiilj
{

	shader_resource::shader_resource(unsigned type, const std::string& path)
		: path_(path), shader_id_(0), type_(type), shader_state_(false)
	{ 
		std::string source = load_shader(path);
		shader_state_ = compile_shader(type, source);
	}

	std::string shader_resource::load_shader(const std::string& file_path)
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

	bool shader_resource::compile_shader(const unsigned type, const std::string& source)
	{
		int result;
		shader_id_ = glCreateShader(type);

		const char* c_str = source.c_str();

		glShaderSource(shader_id_, 1, &c_str, nullptr);
		glCompileShader(shader_id_);

		return debug_shader(shader_id_, shader_debug_type::shader, 
				GL_COMPILE_STATUS, std::cerr, "Failed to compile " + path_ + "!");

	}

	bool shader_resource::recompile_shader()
	{
		if (!path_.empty() && shader_state_ != 0)
		{
			std::string source = load_shader(path_);
			shader_state_ = compile_shader(type_, source);
			return shader_state_;
		}	
	}

	bool shader_resource::attach(unsigned program)
	{ 
		if (!shader_state_)
			return false;

		glAttachShader(program, shader_id_);
		return true;
	}

	void shader_resource::detach(unsigned program)
	{ 
		glDetachShader(program, shader_id_); 
	}

	void shader_resource::free()
	{
		glDeleteShader(shader_id_);
	}

	bool shader_resource::debug_shader(
		const unsigned id, 
		const shader_debug_type type, 
		const unsigned status, 
		std::ostream& stream, 
		const std::string& header)
	{
		int result = 0;
		int log_size = 0;
		char* message;

		switch (type)
		{
		case shader_debug_type::shader:
			glGetShaderiv(id, status, &result);
			if (result == GL_FALSE)
			{
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_size);
				message = static_cast<char*>(alloca(log_size * sizeof(char)));
				glGetShaderInfoLog(id, log_size, nullptr, message);
			}
			break;
		case shader_debug_type::program:
			glGetProgramiv(id, status, &result);
			if (result == GL_FALSE)
			{
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
				message = static_cast<char*>(alloca(log_size * sizeof(char)));
				glGetProgramInfoLog(id, log_size, nullptr, message);
			}
			break;
		default:
			break;
		}

		if (result == GL_FALSE)
			stream << "DEBUG: " << header << "\n" << message << std::endl;

		return result == GL_TRUE;
	}	

	shader_resource::~shader_resource()
	{ }
}
