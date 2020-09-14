#include "program.h"

#include <GL/glew.h>

namespace efiilj
{
	shader_program::shader_program(shader_resource vs, shader_resource fs)
		: vs_(vs), fs_(fs), program_id_(0) 
	{
		create_program(); 
	}

	bool shader_program::create_program()
	{
		program_id_ = glCreateProgram();

		if (vs_.attach(program_id_) && fs_.attach(program_id_))
		{
			glLinkProgram(program_id_);
			if (!shader_resource::debug_shader(program_id_, shader_resource::type_program, 
					GL_LINK_STATUS, std::cout, "Program linking failed!"))
				return false;

			glValidateProgram(program_id_);
			if (!shader_resource::debug_shader(program_id_, shader_resource::type_program, 
					GL_VALIDATE_STATUS, std::cout, "Program validation failed!"))
				return false;
			
			vs_.detach(program_id_);
			vs_.free();

			fs_.detach(program_id_);
			vs_.free();

			return true;
		}

		return false;
	}

	int shader_program::find_uniform_location(const char* name)
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

	bool shader_program::set_uniform(const char* name, const int val)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform1i(uniform, val);
		return true;
	}

	bool shader_program::set_uniform(const char* name, const float val)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform1f(uniform, val);
		return true;
	}

	bool shader_program::set_uniform(const char* name, const vector4& vec)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform4fv(uniform, 1, &vec.at(0));
		return true;
	}

	bool shader_program::set_uniform(const char* name, const matrix4& mat)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniformMatrix4fv(uniform, 1, GL_TRUE, &mat.at(0));
		return true;
	}

	bool shader_program::set_uniform(const char* name, const vector3& vec)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform3fv(uniform, 1, &vec.at(0));
		return true;
	}

	bool shader_program::set_uniform(const char* name, const matrix3& mat)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniformMatrix3fv(uniform, 1, GL_TRUE, &mat.at(0));
		return true;
	}
}
