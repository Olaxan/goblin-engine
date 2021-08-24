#pragma once

#include "ifmgr.h"
#include "manager.h"

#include <memory>
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

		public:

			manager_host();
			~manager_host();

			void register_manager(std::shared_ptr<registrable> mgr, int fcc);
			
			template<class T>
			std::shared_ptr<T> get_manager_from_fcc(int fcc)
			{
				assert(("Manager FourCC invalid!", _reg.find(fcc) != _reg.end()));
				return std::dynamic_pointer_cast<T>(_reg[fcc]);
			}
	};
}
