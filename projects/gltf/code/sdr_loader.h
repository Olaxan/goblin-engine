#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace efiilj
{
	class shader_processor
	{
		private:

			static std::string load_source(const fs::path& path);
			static bool debug_shader(unsigned id, unsigned status, unsigned type, char* msg);
			static unsigned int parse_shader_type(const std::string& str);
			static void parse_include(const fs::path& path, std::string& amal);
			static void parse_source(unsigned int pid, const std::string& amal);

		public:

			static unsigned int compile(const fs::path& path);

	};
}
