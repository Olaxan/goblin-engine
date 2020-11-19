#pragma once

#include "shader_res.h"
#include "camera.h"
#include "matrix4.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace efiilj
{
	class shader_program
	{
	private:

		std::vector<std::shared_ptr<shader_resource>> shaders_;
		std::unordered_map<std::string, int> locations_;

		unsigned program_id_;
		bool program_state_;

		static unsigned active_program_;

	public:

		shader_program();
		shader_program(
				std::shared_ptr<shader_resource> vs,
				std::shared_ptr<shader_resource> fs);

		void add_shader(std::shared_ptr<shader_resource> shader);

		bool create_program();

		bool link();
		bool validate();

		bool reload();

		bool use();
		void drop() const;

		int find_uniform_location(std::string name, bool is_block=false);

		bool bind_block(std::string name, int index);
		
		bool set_uniform(std::string name, int val);
		
		bool set_uniform(std::string name, float val);

		bool set_uniform(std::string name, const vector4& vec);
		
		bool set_uniform(std::string name, const matrix4& mat);
		
		bool set_uniform(std::string name, const vector3& vec);
		
		bool set_uniform(std::string name, const matrix3& mat);
	};
}
