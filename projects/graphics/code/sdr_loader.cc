#include "sdr_loader.h"
#include "regex_exts.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include <GL/glew.h>

#define RECURSION_DEPTH 50

namespace efiilj
{

	std::string shader_processor::load_source(const fs::path& path)
	{
		printf("Loading shader from %s\n", path.string().c_str());

		std::ifstream t(path);
		std::stringstream buffer;

		if (!t.is_open())
		{
			fprintf(stderr, "I/O error - shader not loaded\n");
			return buffer.str();
		}
		
		buffer << t.rdbuf();
		std::string s = buffer.str();

		return s;
	}

	bool shader_processor::debug_shader(unsigned id, unsigned status, unsigned type, char* msg)
	{
		int result = 0;
		int log_size = 0;

		switch (type)
		{
			case GL_SHADER:
			{
				glGetShaderiv(id, status, &result);
				if (result == GL_FALSE)
				{
					glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_size);
					msg = static_cast<char*>(malloc(log_size * sizeof(char)));
					glGetShaderInfoLog(id, log_size, nullptr, msg);
				}
				break;
			}

			case GL_PROGRAM:
			{
				glGetProgramiv(id, status, &result);
				if (result == GL_FALSE)
				{
					glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
					msg = static_cast<char*>(malloc(log_size * sizeof(char)));
					glGetProgramInfoLog(id, log_size, nullptr, msg);
				}
				break;
			}

			default:
				break;
		}

		return result == GL_FALSE;
	}

	unsigned int shader_processor::parse_shader_type(const std::string& type)
	{
		if (type.compare("VERTEX_SHADER") == 0)
			return GL_VERTEX_SHADER;
		if (type.compare("FRAGMENT_SHADER") == 0)
			return GL_FRAGMENT_SHADER;

		return 0;
	}

	void shader_processor::parse_include(const fs::path& path, std::string& str)
	{
		std::regex includes("Include\\((.*)\\)");
		std::smatch matches;

		int i = 0;
		while (std::regex_search(str, matches, includes))
		{
			str = exts::regex_replace(str, includes, 
					[&path](const std::smatch& s) 
					{ 
						return load_source(path / s[1].str()); 
					});

			if (i++ > RECURSION_DEPTH)
			{
				fprintf(stderr, "Error: Reached recursion depth (%d)\n", RECURSION_DEPTH);
				break;
			}
		}
	}

	void shader_processor::parse_source(unsigned int pid, const std::string& amal)
	{
		std::regex source("Begin\\((\\w*)\\)((?:.|\\n)*?)End\\(\\)",
				std::regex_constants::ECMAScript);

    	auto source_begin = std::sregex_iterator(amal.begin(), amal.end(), source);
    	auto source_end = std::sregex_iterator();

		printf("Found %ld sources in shader amalgam\n", std::distance(source_begin, source_end));
		
		for (std::sregex_iterator i = source_begin; i != source_end; ++i) 
		{
			std::smatch match = *i;
			std::string type_str = match[1].str();
			std::string src_str = match[2].str();

			char* msg;
			unsigned int type = parse_shader_type(type_str);

			if (type == 0)
			{
				fprintf(stderr, "Invalid shader type %s\n", type_str.c_str());
				continue;
			}

			unsigned int sid = glCreateShader(type);
			const char* src = src_str.c_str();

			glShaderSource(sid, 1, &src, nullptr);
			glCompileShader(sid);

			if (debug_shader(sid, GL_COMPILE_STATUS, GL_SHADER, msg))
			{
				fprintf(stderr, "Shader compilation error: \n%s\nSource: \n%s\n", msg, src);
				free(msg);
				continue;
			}

			glAttachShader(pid, sid);
    	}
	}

	unsigned int shader_processor::compile(const fs::path& path)
	{
		unsigned int pid = glCreateProgram();

		std::string s = load_source(path);
		parse_include(path.parent_path(), s);
		parse_source(pid, s);

		glLinkProgram(pid);

		char* msg;

		if (debug_shader(pid, GL_LINK_STATUS, GL_PROGRAM, msg))
		{
			fprintf(stderr, "Shader link error: \n%s\n", msg);
			free(msg);
			return 0;
		}

		return pid;
	}
}
