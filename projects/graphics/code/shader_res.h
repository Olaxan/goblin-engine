#pragma once

#include <string>

namespace efiilj
{

	/**
	 * \brief Class to create a shader on the GPU, as well as to hold handles which enables binding.
	 */
	class shader_resource
	{
	private:	

		unsigned shader_id_;
		unsigned type_;
		int shader_state_;
		
		const char* path_;

	public:

		enum class shader_debug_type
		{
			type_shader,
			type_program
		};

		/**
		 * \brief Creates an empty shader resource object.
		 */
		shader_resource();
		
		/**
		 * \brief Creates a shader program from the specified source code.
		 * \param vertex Vertex shader source code as null-terminated char array
		 * \param fragment Fragment shader source code as null-terminated char array
		 */
		shader_resource(unsigned type, const char* path);
		
		/**
		 * \brief Creates a shader program from the specified source code.
		 * \param vertex Vertex shader source code as std::string
		 * \param fragment Fragment shader source code as std::string
		 */
		shader_resource(unsigned type, const std::string& path);

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
		int compile_shader(unsigned int type, const char* source);

		int recompile_shader();

		bool attach(unsigned program); 
		void detach(unsigned program);
		void free();
		
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

		~shader_resource();
	};
}
