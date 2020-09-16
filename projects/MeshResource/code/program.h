#pragma once

#include "shader_res.h"
#include "camera.h"
#include "matrix4.h"

#include <unordered_map>

namespace efiilj
{
	class shader_program
	{
	private:
		shader_resource vs_;
		shader_resource fs_;

		unsigned program_id_;

		std::unordered_map<std::string, int> locations_;

	public:

		shader_program(shader_resource vs, shader_resource fs);

		/**
		 * \brief Creates a shader program from vertex and fragment shader source code
		 * \param vertex The vertex shader source code
		 * \param fragment The fragment shader source code
		 * \return A handle for the shader program on the GPU
		 */
		bool create_program();

		/**
		 * \brief Sets the shader resources as active on the GPU.
		 */

		void use();
		/**
		 * \brief Disables all shaders on the GPU.
		 */
		void drop() const;

		/**
		 * \brief Finds a uniform location within the shader program and returns it. Cached for speed.
		 * \param name The name of the uniform
		 * \return The uniform location, or -1 if not found
		 */
		int find_uniform_location(const char* name, bool is_block=false);

		bool bind_block(const char* name, int index);
		
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
	};
}
