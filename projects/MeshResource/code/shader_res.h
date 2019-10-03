#pragma once

#include <string>
#include <unordered_map>

#include "matrix4.h"

namespace efiilj
{

	/**
	 * \brief Class to create a shader on the GPU, as well as to hold handles which enables binding.
	 */
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

		/**
		 * \brief Creates an empty shader resource object.
		 */
		shader_resource();
		
		/**
		 * \brief Creates a shader program from the specified source code.
		 * \param vertex Vertex shader source code as null-terminated char array
		 * \param fragment Fragment shader source code as null-terminated char array
		 */
		shader_resource(const char* vertex, const char* fragment);
		
		/**
		 * \brief Creates a shader program from the specified source code.
		 * \param vertex Vertex shader source code as std::string
		 * \param fragment Fragment shader source code as std::string
		 */
		shader_resource(const std::string& vertex, const std::string& fragment);

		shader_resource(shader_resource& copy)
			= default;

		shader_resource(shader_resource&& move)
			= default;

		/**
		 * \brief Loads shader source code from the specified file.
		 * \param file_path Path to the source file
		 * \return A std::string containing the shader source
		 */
		static std::string load_shader(const char* file_path);
		
		/**
		 * \brief Loads shader source code from the specified file.
		 * \param file_path Path to the source file
		 * \return A std::string containing the shader source
		 */
		static std::string load_shader(const std::string& file_path);

		/**
		 * \brief Compiles source code of the specified type
		 * \param type Type of shader to compile (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...)
		 * \param source Source code as a null-terminated char array
		 * \return A handle for the compiled shader on the GPU
		 */
		static unsigned int compile_shader(unsigned int type, const char* source);
		
		/**
		 * \brief Creates a shader program from vertex and fragment shader source code
		 * \param vertex The vertex shader source code
		 * \param fragment The fragment shader source code
		 * \return A handle for the shader program on the GPU
		 */
		static unsigned int create_program(const char* vertex, const char* fragment);
		
		/**
		 * \brief Queries the shader error log for the specified type and status, and sends it to the specified output stream.
		 * \param id The shader/program id to debug
		 * \param type Which error log should be queried (shader / program)
		 * \param status The type of error to query for (GL_LINK_STATUS, GL_VALIDATE_STATUS...)
		 * \param stream The output stream to which the result should be sent
		 * \param header An optional string which should be prepended as a header before the output
		 * \return True if the operation was successful; false otherwise
		 */
		static bool debug_shader(unsigned int id, shader_debug_type type, unsigned int status, std::ostream& stream,
		                 const char* header);

		/**
		 * \brief Sets the shader resources as active on the GPU.
		 */
		void use() const;
		/**
		 * \brief Disables all shaders on the GPU.
		 */
		static void drop();

		/**
		 * \brief Finds a uniform location within the shader program and returns it. Cached for speed.
		 * \param name The name of the uniform
		 * \return The uniform location, or -1 if not found
		 */
		int find_uniform_location(const char* name);
		
		/**
		 * \brief Sets an integer uniform on the shader program
		 * \param name The name of the uniform
		 * \param val The integer value which should be set
		 * \return True if successful; false otherwise
		 */
		bool set_uniform(const char* name, int val);
		
		/**
		 * \brief Sets a float uniform on the shader program
		 * \param name The name of the uniform
		 * \param val The float value which should be set
		 * \return True if successful; false otherwise
		 */
		bool set_uniform(const char* name, float val);
		
		/**
		 * \brief Sets a vector4 (vec4) uniform on the shader program
		 * \param name The name of the uniform
		 * \param vec The vector4 which should be set
		 * \return True if successful; false otherwise
		 */
		bool set_uniform(const char* name, const vector4& vec);
		
		/**
		 * \brief Sets a matrix4 (mat4) uniform on the shader program
		 * \param name The name of the uniform
		 * \param mat The matrix4 value which should be set
		 * \return True if successful; false otherwise
		 */
		bool set_uniform(const char* name, const matrix4& mat);
		
		/**
		 * \brief Sets an vector3 (vec3) uniform on the shader program
		 * \param name The name of the uniform
		 * \param vec The vector3 value which should be set
		 * \return True if successful; false otherwise
		 */
		bool set_uniform(const char* name, const vector3& vec);
		
		/**
		 * \brief Sets an matrix3 (mat3) uniform on the shader program
		 * \param name The name of the uniform
		 * \param mat The matrix3 value which should be set
		 * \return True if successful; false otherwise
		 */
		bool set_uniform(const char* name, const matrix3& mat);
		

		~shader_resource();
	};
}
