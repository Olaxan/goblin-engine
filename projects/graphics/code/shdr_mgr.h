#pragma once

#include "manager.h"
#include "mgr_host.h"
#include "matrix4.h"

#include <string>
#include <filesystem>

namespace efiilj
{
	typedef int shader_id;

	class shader_server : public server<shader_id>
	{
		private:

			mutable shader_id _current;

			struct ShaderData
			{
				std::vector<std::filesystem::path> uri;
				std::vector<unsigned int> program_id;
				std::vector<unsigned int> type;
				std::vector<bool> state;
			} _data;

		public:

			shader_server();
			~shader_server();

			void append_defaults(shader_id idx) override;
			bool is_valid(shader_id idx) const override;

			void on_register(std::shared_ptr<manager_host> host) override;

			bool compile(shader_id idx);
			bool use(shader_id idx) const;

			int find_uniform_location(shader_id idx, const std::string& name, bool is_block = false);
			bool bind_block(shader_id idx, const std::string& name, unsigned block); 
		
			bool set_uniform(shader_id idx, const std::string& name, int val);
			bool set_uniform(shader_id idx, const std::string& name, unsigned val);
			bool set_uniform(shader_id idx, const std::string& name, float val);
			bool set_uniform(shader_id idx, const std::string& name, const vector4& vec);
			bool set_uniform(shader_id idx, const std::string& name, const matrix4& mat);
			bool set_uniform(shader_id idx, const std::string& name, const vector3& vec);
			bool set_uniform(shader_id idx, const std::string& name, const matrix3& mat);

			bool set_uniform(const std::string& name, int val);
			bool set_uniform(const std::string& name, unsigned val);
			bool set_uniform(const std::string& name, float val);
			bool set_uniform(const std::string& name, const vector4& vec);
			bool set_uniform(const std::string& name, const matrix4& mat);
			bool set_uniform(const std::string& name, const vector3& vec);
			bool set_uniform(const std::string& name, const matrix3& mat);

			const std::filesystem::path& get_uri(shader_id idx) const;
			void set_uri(shader_id idx, const std::filesystem::path& uri);

			const unsigned int& get_program_id(shader_id idx) const
			{
				return _data.program_id[idx];
			}
	};
}
