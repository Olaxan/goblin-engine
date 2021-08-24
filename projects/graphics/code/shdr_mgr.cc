#include "shdr_mgr.h"
#include "sdr_loader.h"

#include <GL/glew.h>

namespace efiilj
{
	shader_server::shader_server()
	{
		printf("Init shaders...\n");
	}

	shader_server::~shader_server()
	{
		printf("Shader server exit\n");
	}

	shader_id shader_server::create() 
	{
		shader_id new_id = _pool.size();
		_pool.emplace_back(new_id);

		_data.program_id.emplace_back(0);
		_data.state.emplace_back(false);
		_data.type.emplace_back(0);
		_data.uri.emplace_back();
		
		return new_id;
	}

	bool shader_server::destroy(shader_id idx)
	{
		return false;
	}

	void shader_server::on_register(std::shared_ptr<manager_host> host)
	{
		
	}

	bool shader_server::is_valid(shader_id idx) const
	{
		return _data.state[idx];
	}

	bool shader_server::use(shader_id idx) const
	{
		if (!is_valid(idx))
			return false;

		glUseProgram(_data.program_id[idx]);
		return true;
	}

	bool shader_server::compile(shader_id idx)
	{
		unsigned int pid = shader_processor::compile(_data.uri[idx]);

		if (pid > 0)
		{
			_data.program_id[idx] = pid;
			_data.state[idx] = true;
			return true;
		}

		fprintf(stderr, "Compilation failed\n");
		return false;
	}

	int shader_server::find_uniform_location(shader_id idx, const std::string& name, bool is_block)
	{
		int uniform = -1;

		unsigned prog = _data.program_id[idx];

		uniform = is_block ? 
			glGetUniformBlockIndex(prog, name.c_str()) : glGetUniformLocation(prog, name.c_str());

		return uniform;
	}

	bool shader_server::bind_block(shader_id idx, const std::string& name, unsigned block)
	{
		const int uniform = find_uniform_location(idx, name, true);

		if (uniform == -1)
			return false;

		glUniformBlockBinding(_data.program_id[idx], uniform, block);
		return true;
	}

	bool shader_server::set_uniform(shader_id idx, const std::string& name, int val)
	{
		const int uniform = find_uniform_location(idx, name);

		if (uniform == -1)
			return false;

		glUniform1i(uniform, val);
		return true;
	}

	bool shader_server::set_uniform(shader_id idx, const std::string& name, float val)
	{
		const int uniform = find_uniform_location(idx, name);

		if (uniform == -1)
			return false;

		glUniform1f(uniform, val);
		return true;
	}

	bool shader_server::set_uniform(shader_id idx, const std::string& name, const vector4& vec)
	{
		const int uniform = find_uniform_location(idx, name);

		if (uniform == -1)
			return false;

		glUniform4fv(uniform, 1, &vec.at(0));
		return true;
	}

	bool shader_server::set_uniform(shader_id idx, const std::string& name, const matrix4& mat)
	{
		const int uniform = find_uniform_location(idx, name);

		if (uniform == -1)
			return false;

		glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat.at(0));
		return true;
	}

	bool shader_server::set_uniform(shader_id idx, const std::string& name, const vector3& vec)
	{
		const int uniform = find_uniform_location(idx, name);

		if (uniform == -1)
			return false;

		glUniform3fv(uniform, 1, &vec.at(0));
		return true;
	}

	bool shader_server::set_uniform(shader_id idx, const std::string& name, const matrix3& mat)
	{
		const int uniform = find_uniform_location(idx, name);

		if (uniform == -1)
			return false;

		glUniformMatrix3fv(uniform, 1, GL_FALSE, &mat.at(0));
		return true;
	}

	const std::filesystem::path& shader_server::get_path(shader_id idx) const
	{
		return _data.uri[idx];
	}

	void shader_server::set_uri(shader_id idx, const std::filesystem::path& uri)
	{
		_data.uri[idx] = uri;
	}
}
