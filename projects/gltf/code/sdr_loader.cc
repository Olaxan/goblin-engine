#include "sdr_loader.h"
#include "regex_exts.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#define RECURSION_DEPTH 50

namespace efiilj
{
	std::string shader_processor::load(const fs::path& path)
	{
		printf("Loading shader from %s\n", path.string().c_str());

		std::ifstream t(path);
		std::stringstream buffer;

		if (!t.is_open())
		{
			fprintf(stderr, "I/O error - shader not loaded\n");
			return buffer.str();
		}
		
		buffer << t.rdbuf();
		std::string s = buffer.str();

		parse_include(path.parent_path(), s);

		return s;
	}

	void shader_processor::parse_include(const fs::path& path, std::string& str)
	{
		std::regex includes("Include\\((.*)\\)");
		std::smatch matches;

		int i = 0;
		while (std::regex_search(str, matches, includes))
		{
			str = exts::regex_replace(str, includes, 
					[&path](const std::smatch& s) 
					{ 
						return load(path / s[1].str()); 
					});

			if (i++ > RECURSION_DEPTH)
			{
				fprintf(stderr, "Error: Reached recursion depth (%d)\n", RECURSION_DEPTH);
				break;
			}
		}
	}
}
