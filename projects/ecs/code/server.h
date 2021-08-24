#pragma once

#include <vector>

namespace efiilj
{
	template<class T>
	class server
	{
		protected:

			std::vector<T> _pool;

		public:

			virtual T create() = 0;
			virtual bool destroy(T id) = 0;
	};
}
