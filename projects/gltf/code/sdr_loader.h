#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace efiilj
{
	class shader_processor
	{
		public:

			static std::string load(const fs::path& path);
			static void parse_include(const fs::path& path, std::string& str);
	};
}
