#pragma once

#include "ifmgr.h"

#include <memory>
#include <vector>
#include <cassert>
#include <unordered_map>

#include "stdio.h"

namespace efiilj
{

	union fourcc
	{
		int code;
		char name[4];
		struct { char a, b, c, d; };
	};

	class manager_host : public std::enable_shared_from_this<manager_host>
	{
		private:

			std::unordered_map<int, std::shared_ptr<registrable>> _reg;
			std::vector<std::shared_ptr<component_base>> _components;
			std::vector<std::shared_ptr<server_base>> _servers;

		public:

			manager_host();
			~manager_host();

			void register_manager(std::shared_ptr<registrable> mgr, int fcc);
			
			template<class T>
			std::shared_ptr<T> get_manager_from_fcc(int fcc)
			{
				assert(("Manager FourCC invalid!", _reg.find(fcc) != _reg.end())); //NOLINT
				return std::dynamic_pointer_cast<T>(_reg[fcc]);
			}

			const std::vector<std::shared_ptr<component_base>>& get_components()
			{
				return _components;
			}
			
			const std::vector<std::shared_ptr<server_base>>& get_servers()
			{
				return _servers;
			}

			void on_setup();
			void on_begin_frame();
			void on_frame();
			void on_end_frame();
	};
}
