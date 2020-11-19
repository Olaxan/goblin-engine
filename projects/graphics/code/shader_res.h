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
		bool shader_state_;
		
		const std::string path_;

	public:

		enum class shader_debug_type
		{
			shader,
			program
		};

		shader_resource(unsigned type, const std::string& path);

		shader_resource(shader_resource& copy)
			= default;

		shader_resource(shader_resource&& move)
			= default;	
		
		static std::string load_shader(const std::string& file_path);

		bool compile_shader(unsigned int type, const std::string& source);

		bool recompile_shader();

		bool attach(unsigned program); 
		void detach(unsigned program);
		void free();
		
		static bool debug_shader(unsigned id, shader_debug_type type, 
				unsigned status, std::ostream& stream, const std::string& header);

		~shader_resource();
	};
}
