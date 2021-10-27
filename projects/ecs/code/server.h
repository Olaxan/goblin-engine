#pragma once

#include "ifmgr.h"

#include <vector>

namespace efiilj
{
	template<class T>
	class server : public server_base
	{
		protected:

			std::vector<T> _pool;
			std::vector<bool> _alive;

			virtual void append_defaults(T) = 0;

		public:

			virtual T create()
			{
				T new_id = static_cast<T>(_pool.size());

				_pool.emplace_back(new_id);
				_alive.emplace_back(true);

				append_defaults(new_id);

				return new_id;
			}

			virtual void on_editor_gui(T) {}

			virtual bool destroy(T id)
			{
				_alive[id] = false;
				return true;
			}

			virtual bool is_valid(T id) const
			{
				return (id >= 0 && id < static_cast<int>(_pool.size()) && _alive[id]);
			}

			const std::vector<T>& get_ids()
			{
				return _pool;
			}
	};
}
