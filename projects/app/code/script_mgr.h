#pragma once
#include "manager.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

namespace efiilj
{
	typedef int script_id;

	class script_manager : public manager<script_id>
	{
		private:

			struct ScriptData
			{
				ComponentData<std::string> path;
			} _data;

			py::scoped_interpreter guard { };

		public:

			script_manager();
			~script_manager();

			void on_setup() override;
			void on_register(std::shared_ptr<manager_host>) override;

			void set_uri(script_id idx, const std::string& uri)
			{ _data.path[idx] = uri; }
	};
}
