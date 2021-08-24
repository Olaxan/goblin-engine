#pragma once

#include "manager.h"
#include "mgr_host.h"

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace efiilj
{
	typedef int shader_id;

	class shader_server : public registrable
	{
		private:

			struct ShaderData
			{
				std::vector<fs::path> uri;
				std::vector<unsigned int> program_id;
				std::vector<unsigned int> type;
				std::vector<bool> state;
			} _data;

		public:

			shader_server();
			~shader_server();

			void on_register(std::shared_ptr<manager_host> host) override;

	};
}
