#include "program.h"

#include <GL/glew.h>

namespace efiilj
{
	unsigned shader_program::active_program_ = 0;

	shader_program::shader_program(shader_resource vs, shader_resource fs)
		: vs_(vs), fs_(fs), program_id_(0), program_state_(false)
	{
		if (create_program())
			printf("Created shader program %d\n", program_id_);
		else
			printf("Error: Failed to create program!\n");
	}

	bool shader_program::create_program()
	{
		program_id_ = glCreateProgram();
		program_state_ = false;

		if (vs_.attach(program_id_) && fs_.attach(program_id_))
		{
			glLinkProgram(program_id_);
			if (!shader_resource::debug_shader(program_id_, shader_resource::shader_debug_type::type_program, 
					GL_LINK_STATUS, std::cout, "Program linking failed!"))
				return false;
			
			glValidateProgram(program_id_);
			if (!shader_resource::debug_shader(program_id_, shader_resource::shader_debug_type::type_program,
					GL_VALIDATE_STATUS, std::cout, "Program validation failed!"))
				return false;
			
			vs_.detach(program_id_);
			vs_.free();

			fs_.detach(program_id_);
			fs_.free();

			program_state_ = true;

			return true;
		}

		return false;
	}

	bool shader_program::reload()
	{
		glDeleteProgram(program_id_);
		vs_.recompile_shader();
		fs_.recompile_shader();
		return create_program();	
	}

	bool shader_program::use()
	{
		if (active_program_ == program_id_)
			return true;

		if (program_state_)
		{
			glUseProgram(program_id_);
			active_program_ = program_id_;
			return true;
		}
		else 
		{
			glUseProgram(0);
			active_program_ = 0;
			return false;
		}
	}

	void shader_program::drop() const
	{
		glUseProgram(0);
		active_program_ = 0;
	}

	int shader_program::find_uniform_location(std::string name, bool is_block)
	{
		int uniform;
		const auto it = locations_.find(name);

		if (it == locations_.end())
		{
			uniform = is_block ? glGetUniformBlockIndex(program_id_, name.c_str()) : glGetUniformLocation(program_id_, name.c_str());
			if (uniform != -1)
				locations_[name] = uniform;
		}
		else
			uniform = it->second;
		
		return uniform;
	}

	bool shader_program::bind_block(std::string name, const int index)
	{
		const int uniform = find_uniform_location(name, true);
		if (uniform == -1)
			return false;

		glUniformBlockBinding(program_id_, uniform, index);
		return true;
	}

	bool shader_program::set_uniform(std::string name, const int val)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform1i(uniform, val);
		return true;
	}

	bool shader_program::set_uniform(std::string name, const float val)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform1f(uniform, val);
		return true;
	}

	bool shader_program::set_uniform(std::string name, const vector4& vec)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform4fv(uniform, 1, &vec.at(0));
		return true;
	}

	bool shader_program::set_uniform(std::string name, const matrix4& mat)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat.at(0));
		return true;
	}

	bool shader_program::set_uniform(std::string name, const vector3& vec)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniform3fv(uniform, 1, &vec.at(0));
		return true;
	}

	bool shader_program::set_uniform(std::string name, const matrix3& mat)
	{
		const int uniform = find_uniform_location(name);
		if (uniform == -1)
			return false;

		glUniformMatrix3fv(uniform, 1, GL_FALSE, &mat.at(0));
		return true;
	}
}
