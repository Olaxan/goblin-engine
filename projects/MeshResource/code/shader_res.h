#pragma once

#include <string>
#include <unordered_map>

#include "matrix4.h"

namespace efiilj
{
	/// <summary>
	/// Class to create a shader on the GPU, as well as to hold handles which enables binding.
	/// </summary>
	class shader_resource
	{
	private:

		enum shader_debug_type
		{
			type_shader,
			type_program
		};

		unsigned int program_id_;

		std::unordered_map<std::string, int> locations_;

	public:

		shader_resource();
		shader_resource(const char* vertex, const char* fragment);
		shader_resource(const std::string& vertex, const std::string& fragment);

		static std::string load_shader(const char* file_path);
		static std::string load_shader(const std::string& file_path);

		static unsigned int compile_shader(unsigned int type, const char* source);
		static unsigned int create_program(const char* vertex, const char* fragment);
		static bool debug_shader(unsigned int id, shader_debug_type type, unsigned int status, std::ostream& stream,
		                 const char* header);

		void use() const;
		static void drop();

		int find_uniform_location(const char* name);
		bool set_uniform1_i(const char* name, int val);
		bool set_uniform_vector4_fv(const char* name, const Vector4& vec);
		bool set_uniform_matrix4_fv(const char* name, const Matrix4& mat);

		~shader_resource();
	};
}
